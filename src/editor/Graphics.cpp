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
    Vertexbuffer::Vertexbuffer(const float* vertices, std::size_t size)
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
        logMsg("Vertex buffer created with " + std::to_string(size) + " vertices");
    }

    Vertexbuffer::~Vertexbuffer()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
    }

    Vertexbuffer::Vertexbuffer(Vertexbuffer&& vertexbuffer)
    {
        m_vao = vertexbuffer.m_vao;
        vertexbuffer.m_vao = 0;
        m_vbo = vertexbuffer.m_vbo;
        vertexbuffer.m_vbo = 0;
    }

    Vertexbuffer &Vertexbuffer::operator=(Vertexbuffer&& vertexbuffer)
    {
        m_vao = vertexbuffer.m_vao;
        vertexbuffer.m_vao = 0;
        m_vbo = vertexbuffer.m_vbo;
        vertexbuffer.m_vbo = 0;
        return *this;
    }

    void Vertexbuffer::bind()
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

    Framebuffer::Framebuffer(Framebuffer&& framebuffer)
    {
        m_framebufferID = framebuffer.m_framebufferID;
        framebuffer.m_framebufferID = 0;
        for (int i = 0; i < framebuffer.m_numColorAttachments; ++i)
        {
            m_colorAttachments[i] = std::move(framebuffer.m_colorAttachments[i]);
        }
        m_numColorAttachments = framebuffer.m_numColorAttachments;
        framebuffer.m_numColorAttachments = 0;
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& framebuffer)
    {
        m_framebufferID = framebuffer.m_framebufferID;
        framebuffer.m_framebufferID = 0;
        for (int i = 0; i < framebuffer.m_numColorAttachments; ++i)
        {
            m_colorAttachments[i] = std::move(framebuffer.m_colorAttachments[i]);
        }
        m_numColorAttachments = framebuffer.m_numColorAttachments;
        framebuffer.m_numColorAttachments = 0;
        return *this;
    }

    void Framebuffer::addColorAttachment(RenderTexture&& renderTexture)
    {
        if (m_numColorAttachments < sizeof(m_colorAttachments))
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_numColorAttachments, renderTexture.id(), 0);
            m_colorAttachments[m_numColorAttachments] = std::move(renderTexture);
            logMsg("Color attachment " + std::to_string(m_numColorAttachments) + " added to framebuffer");
            m_numColorAttachments++;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            GLenum drawBuffers[16];

            const unsigned int numAttachments = getNumColorAttachments();
            for (int i = 0; i < numAttachments; ++i)
            {
                drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
            }
            glDrawBuffers(numAttachments, drawBuffers);
        }
        else
        {
            logWarning("Color attachments already full! New color attachment not added");
        }
    }

    void Framebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
    }

    void Framebuffer::unbind()
    {
        // bind "default" framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::clear()
    {
        const GLint clearColor[4] = {0, 0, 0, 0};
        glClearBufferiv(GL_COLOR, 0, clearColor);
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
        logMsg("RenderTexture created with: size: " + std::to_string(width) + "x" + std::to_string(height) + ", depth test: " + std::to_string(depthTesting));
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
            logMsg("Successfully linked shaders, ready for execution");
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

    Camera::Camera(float aspectRatio, glm::vec3 pos, glm::vec3 target) :
            pos(pos), m_aspectRatio(aspectRatio)
    {
        update();
    }

    void Camera::update()
    {
        m_dir.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        m_dir.y = glm::sin(glm::radians(pitch));
        m_dir.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

        m_right = glm::normalize(glm::cross({0.0f, 1.0f, 0.0f}, -m_dir));
        m_up = glm::cross(-m_dir, m_right);
        m_view = glm::lookAt(pos, pos + m_dir, m_up);
        m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_zNear, m_zFar);
    }

    void drawToTexture(Vertexbuffer& vertexBuffer, ShaderProgram& shader, Framebuffer& framebuffer)
    {
        // We assume that all color attachments are same size
        glViewport(0, 0, framebuffer.getColorAttachments()[0].getWidth(), framebuffer.getColorAttachments()[0].getHeight());

        vertexBuffer.bind();
        shader.use();
        framebuffer.bind();
        framebuffer.clear();

        glDrawArrays(GL_TRIANGLES, 0, 36);

        framebuffer.unbind();

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

        logMsg("Loaded texture " + std::string(filename) + ", with size: " + std::to_string(gltexture.width) + "x" + std::to_string(gltexture.height));

        return { gltexture };
    }

    void clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void setUniform(unsigned int shader, const char* name, glm::vec3 v)
    {
        int loc = glGetUniformLocation(shader, name);
        glUniform3f(loc, v[0], v[1], v[2]);
    }

    void Editor::setUniform(unsigned int shader, const char* name, glm::vec4 v)
    {
        int loc = glGetUniformLocation(shader, name);
        glUniform4f(loc, v[0], v[1], v[2], v[3]);
    }

    void Editor::setUniform(unsigned int shader, const char* name, glm::mat4 m)
    {
        int loc = glGetUniformLocation(shader, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
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
        char infoLog[logSize];

        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
            logError("---SHADER COMPILE LOG BEGIN---");
            logRaw(infoLog);
            logError("---SHADER COMPILE LOG END---");
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
            glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
            logError("---SHADER LINK LOG BEGIN---");
            logRaw(infoLog);
            logError("---SHADER LINK LOG END---");
            return false;
        }
        return true;
    }
}