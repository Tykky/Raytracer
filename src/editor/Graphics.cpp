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
    VertexBuffer::VertexBuffer(float* vertices, std::size_t size)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

        // Vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TODO: construct index/element buffer

        glBindVertexArray(0);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
    }

    void VertexBuffer::bind()
    {
        glBindVertexArray(m_vao);
    }

    Framebuffer::Framebuffer()
    {
        glGenFramebuffers(1, &m_framebufferID);
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_framebufferID);
    }

    void Framebuffer::addColorAttachment(RenderTexture&& renderTexture)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
        if (m_numColorAttachments < sizeof(m_colorAttachments))
        {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_numColorAttachments, renderTexture.id(), 0);
            m_colorAttachments[m_numColorAttachments] = std::move(renderTexture);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            m_numColorAttachments++;
        }
        else
        {
            logWarning("Color attachments already full! New color attachment not added");
        }
    }

    RenderTexture::RenderTexture(unsigned int width, unsigned height, bool depthTesting) :
            m_width(width), m_height(height), m_isDepthTestEnabled(depthTesting)
    {
        glGenTextures(1, &m_renderTextureId);
        glBindTexture(GL_TEXTURE_2D, m_renderTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (depthTesting)
        {
            glGenRenderbuffers(1, &m_depthRenderBufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }

    RenderTexture::~RenderTexture()
    {
        glDeleteTextures(1, &m_renderTextureId);
        if (m_isDepthTestEnabled)
            glDeleteRenderbuffers(1, &m_renderTextureId);
    }

    RenderTexture::RenderTexture(RenderTexture&& renderTexture)
    {
        m_isDepthTestEnabled = renderTexture.m_isDepthTestEnabled;
        renderTexture.m_isDepthTestEnabled = false;
        m_width = renderTexture.m_width;
        renderTexture.m_width = 0;
        m_height = renderTexture.m_height;
        renderTexture.m_height = 0;
        m_renderTextureId = renderTexture.m_renderTextureId;
        renderTexture.m_renderTextureId = 0;
        m_depthRenderBufferId = renderTexture.m_depthRenderBufferId;
        renderTexture.m_depthRenderBufferId = 0;
    }

    RenderTexture& RenderTexture::operator=(RenderTexture&& renderTexture)
    {
        m_isDepthTestEnabled = renderTexture.m_isDepthTestEnabled;
        renderTexture.m_isDepthTestEnabled = false;
        m_width = renderTexture.m_width;
        renderTexture.m_width = 0;
        m_height = renderTexture.m_height;
        renderTexture.m_height = 0;
        m_renderTextureId = renderTexture.m_renderTextureId;
        renderTexture.m_renderTextureId = 0;
        m_depthRenderBufferId = renderTexture.m_depthRenderBufferId;
        renderTexture.m_depthRenderBufferId = 0;
        return *this;
    }

    ShaderProgram::~ShaderProgram()
    {
        if (m_shaderProgramId)
            glDeleteProgram(m_shaderProgramId);
        if (m_vertexShaderId)
            glDeleteShader(m_vertexShaderId);
        if (m_fragmentShaderId)
            glDeleteShader(m_fragmentShaderId);
        if (m_computeShaderId)
            glDeleteShader(m_computeShaderId);
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)
    {
        m_shaderProgramId = shaderProgram.m_shaderProgramId;
        shaderProgram.m_shaderProgramId = 0;
        m_vertexShaderId = shaderProgram.m_vertexShaderId;
        shaderProgram.m_vertexShaderId = 0;
        m_fragmentShaderId = shaderProgram.m_fragmentShaderId;
        shaderProgram.m_computeShaderId = 0;
        m_computeShaderId = shaderProgram.m_computeShaderId;
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram)
    {
        m_shaderProgramId = shaderProgram.m_shaderProgramId;
        shaderProgram.m_shaderProgramId = 0;
        m_vertexShaderId = shaderProgram.m_vertexShaderId;
        shaderProgram.m_vertexShaderId = 0;
        m_fragmentShaderId = shaderProgram.m_fragmentShaderId;
        shaderProgram.m_computeShaderId = 0;
        m_computeShaderId = shaderProgram.m_computeShaderId;
        return *this;
    }

    bool ShaderProgram::addShader(const char* path, ShaderType shaderType)
    {
        Rawbuffer buf;
        if (readRawFile(buf, path))
        {
            const char* bufPtr = buf.data();
            const int size = buf.size();
            if (unsigned int shader = compileShader(&bufPtr, &size, shaderType))
            {
                logMsg("Successfully compiled shader from path: " + std::string(path));
                switch (shaderType)
                {
                    case ShaderType::VERTEX:
                        m_vertexShaderId = shader;
                        return true;
                    case ShaderType::FRAGMENT:
                        m_fragmentShaderId = shader;
                        return true;
                    case ShaderType::COMPUTE:
                        m_computeShaderId = shader;
                        return true;
                }
            }
            else
            {
                logError("Failed to compile shader from path: " + std::string(path));
            }
        }
        return false;
    }

    bool ShaderProgram::link()
    {
        m_shaderProgramId = glCreateProgram();
        if (m_vertexShaderId)
            glAttachShader(m_shaderProgramId, m_vertexShaderId);
        if (m_fragmentShaderId)
            glAttachShader(m_shaderProgramId, m_fragmentShaderId);
        if (m_computeShaderId)
            glAttachShader(m_shaderProgramId, m_computeShaderId);
        glLinkProgram(m_shaderProgramId);
        if (checkShaderLink(m_shaderProgramId))
        {
            logMsg("Shaders successfully linked into a program");
            // TODO: consider calling delete on shaders when they are linked
            return true;
        }
        else
        {
            logError("Failed to link shaders");
            return false;
        }
    }

    void ShaderProgram::use()
    {
        glUseProgram(m_shaderProgramId);
    }

    unsigned int compileShader(const char** data, const int* size, ShaderType shaderType)
    {
        unsigned int shader = glCreateShader(static_cast<unsigned int>(shaderType));
        glShaderSource(shader, 1, data, size);
        glCompileShader(shader);
        if (checkShaderCompilation(shader))
        {
            return shader;
        }
        else
        {
            glDeleteShader(shader);
            return 0;
        }
    }

    Camera::Camera() :
        m_pos({0.0f, 0.0f, 0.0f}), m_target({0.0f, 0.0f, 0.0f}), m_width(0), m_height(0) {}

    Camera::Camera(unsigned int width, unsigned int height, glm::vec3 pos, glm::vec3 target) :
        m_pos(pos), m_target(target), m_width(width), m_height(height)
    {
        update();
    }

    void Camera::move(glm::vec3 dir)
    {
        m_pos += dir;
    }

    void Camera::update()
    {
        // Gram-Schmidt process
        m_dir = glm::normalize(m_pos - m_target);
        m_right = glm::normalize(glm::cross({0.0f, 1.0f, 0.0f}, m_dir));
        m_up = glm::cross(m_dir, m_right);
        m_view = glm::lookAt(m_pos, m_dir, m_up);
        m_projection = glm::perspective(glm::radians(m_fov), static_cast<float>(m_width) / static_cast<float>(m_height), m_zNear, m_zFar);
    }

    void Camera::render()
    {
        if (m_width == 0 || m_height == 0)
        {
            logWarning("Width or height is 0, nothing will be drawn");
            return;
        }
    }

    void drawAllBuffers(Framebuffer& framebuffer)
    {
        GLenum drawBuffers[16];
        const RenderTexture* colorAttachments = framebuffer.getColorAttachments();
        const unsigned int numAttachments = framebuffer.getNumColorAttachments();
        for (int i = 0; i < numAttachments; ++i)
        {
            drawBuffers[i] = colorAttachments[i].id();
        }
        glDrawBuffers(numAttachments, drawBuffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            logError("Failed to draw to framebuffer");
        }
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