#include "Graphics.h"
#include "logging/Logging.h"

// We need to define this once, so STB_IMAGE implementation gets placed in only one of the translation units
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <GL/glew.h>
#include <stdexcept>
#include <string>

namespace Editor
{
    Framebuffer::Framebuffer()
    {
        glGenFramebuffers(1, &m_framebufferID);
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_framebufferID);
    }

    RenderTexture::RenderTexture(unsigned int width, unsigned height, bool depthTesting) :
        m_width(width), m_height(height), m_depthTestEnabled(depthTesting)
    {
        glGenTextures(1, &m_renderTextureId);
        glBindTexture(GL_TEXTURE_2D, m_renderTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (depthTesting)
        {
            glGenRenderbuffers(1, &m_depthBufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }

    RenderTexture::~RenderTexture()
    {
        glDeleteTextures(1, &m_renderTextureId);
        if (m_depthTestEnabled)
            glDeleteRenderbuffers(1, &m_renderTextureId);
    }

    Shader::Shader(const char* name, const char* filePath, ShaderType shaderType) :
            m_shaderType(shaderType), m_name(name), m_filePath(filePath)
    {
        Rawbuffer source;
        if (readRawFile(source, filePath))
        {
            const char* sourceData = source.data();
            const int sourceLength = source.size();
            m_shaderId = glCreateShader(static_cast<unsigned int>(shaderType));
            glShaderSource(m_shaderId, 1, &sourceData, &sourceLength);
            m_readFileSuccess = true;
            logMsg("Shader loaded: " + m_name);
        }
    }

    Shader::~Shader()
    {
        if (m_readFileSuccess)
            glDeleteShader(m_shaderId);
    }

    bool Shader::compile()
    {
        glCompileShader(m_shaderId);
        if (m_readFileSuccess && checkShaderCompilation(m_shaderId))
        {
            m_compileSuccess = true;
            logMsg("Compiled shader: " + m_name);
            return true;
        }
        logError("Failed to compile shader" + m_name);
        return false;
    }

    Shader::Shader(Shader&& shader)
    {
        m_name = std::move(shader.m_name);
        m_filePath = std::move(shader.m_filePath);
        m_shaderId = shader.m_shaderId;
        m_shaderType = shader.m_shaderType;
        m_readFileSuccess = shader.m_readFileSuccess;
        m_compileSuccess = shader.m_compileSuccess;
        // When readFIleSuccess is false the destructor doesn't do anything
        // TODO this is hacky solution, figure out something better
        shader.m_readFileSuccess = false;
    }

    Shader& Shader::operator=(Shader&& shader)
    {
        m_name = std::move(shader.m_name);
        m_filePath = std::move(shader.m_filePath);
        m_shaderId = shader.m_shaderId;
        m_shaderType = shader.m_shaderType;
        m_readFileSuccess = shader.m_readFileSuccess;
        m_compileSuccess = shader.m_compileSuccess;
        // When readFIleSuccess is false the destructor doesn't do anything
        // TODO this is hacky solution, figure out something better
        shader.m_readFileSuccess = false;
        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderStore* shaders) :
        m_shaders(shaders)
    {
        if (!shaders)
        {
            logError("Failed to create shader program, nullptr given");
            return;
        }

        m_shaderProgramId = glCreateProgram();
        std::string shaderNames = "";

        for (auto& shader : *shaders)
        {
            glAttachShader(m_shaderProgramId, shader.getId());
            shaderNames += shader.getName() + ", ";
        }

        glLinkProgram(m_shaderProgramId);
        if (!checkShaderLink(m_shaderProgramId))
        {
            logError("Failed to link shaders into program: " + std::move(shaderNames));
        }
        logMsg("Linked shaders: " + std::move(shaderNames));
    }

    std::optional<Texture> loadTexture(const char* filename)
    {
        Texture gltexture;
        unsigned char* data = stbi_load(filename, &gltexture.width, &gltexture.height, nullptr, 4);

        if (!data)
        {
            logWarning("Failed to load texture " + std::string(filename));
            return std::nullopt;
        }

        glGenTextures(1, &gltexture.textureID);
        glBindTexture(GL_TEXTURE_2D, gltexture.textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gltexture.width, gltexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        gltexture.name = filename;

        logMsg("Loaded texture " + std::string(filename));

        return { gltexture };
    }

    void clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void deleteTexture(unsigned int textureID)
    {
        glDeleteTextures(1, &textureID);
    }

    void* createTexture(unsigned char* data, int w, int h, char fmt)
    {
        GLuint tex;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return (void*)tex;
    }

    void deleteTexture(void* tex)
    {
        GLuint texID = (GLuint)tex;
        glDeleteTextures(1, &texID);
    }

    bool checkShaderCompilation(unsigned int shaderId)
    {
        int success;
        const int logSize = 1024;
        char infolog[logSize];

        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, logSize, NULL, infolog);
            return false;
        }
        return true;
    }

    bool checkShaderLink(unsigned int shaderId)
    {
        int success;
        const int logSize = 1024;
        char infoLog[logSize];

        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderId, logSize, NULL, infoLog);
            return false;
        }
        return true;
    }
}