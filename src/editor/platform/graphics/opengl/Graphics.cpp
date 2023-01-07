// this needs to be defined once, so STB_IMAGE implementation gets placed in only
// one of the translation units
#define STB_IMAGE_IMPLEMENTATION

#include "Graphics.h"
#include "logging/Logging.h"
#include <stb_image.h>
#include <GL/glew.h>
#include <string>
#include <stdexcept>

namespace Editor
{
    uint TextureTargetToGLEnum(TextureTarget target)
    {
        switch (target)
        {
			case TextureTarget::TEXTURE_1D:
                return GL_TEXTURE_1D;
			case TextureTarget::TEXTURE_2D:
                return GL_TEXTURE_2D;
			case TextureTarget::TEXTURE_3D:
                return GL_TEXTURE_3D;
			case TextureTarget::TEXTURE_1D_ARRAY:
                return GL_TEXTURE_1D_ARRAY;
			case TextureTarget::TEXUTRE_2D_ARRAY:
                return GL_TEXTURE_2D_ARRAY;
			case TextureTarget::TEXTURE_CUBE_MAP:
                return GL_TEXTURE_CUBE_MAP;
			case TextureTarget::TEXTURE_CUBE_MAP_ARRAY:
                return GL_TEXTURE_CUBE_MAP_ARRAY;
        }
    }

    uint FramebufferOperationToGLEnum(FramebufferOperation op)
    {
        switch (op)
        {
			case FramebufferOperation::READ:
                return GL_READ_FRAMEBUFFER;
			case FramebufferOperation::DRAW:
                return GL_DRAW_FRAMEBUFFER;
			case FramebufferOperation::READ_AND_DRAW:
				return GL_FRAMEBUFFER;
        }
    }

    constexpr std::array shaderStages {
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_GEOMETRY_SHADER,
		GL_COMPUTE_SHADER,
		GL_TESS_CONTROL_SHADER,
		GL_TESS_EVALUATION_SHADER,
    };

    Texture createTexture()
    {
        uint id;
        glGenTextures(1, &id);
        return { id };
    }

    void deleteTexture(Texture& texture)
    {
        glDeleteTextures(1, &texture.id);
        texture.id = 0;
    }

    void bindTexture(Texture texture, TextureTarget target)
    {
        if (!texture.id)
            throw std::runtime_error("Tried to bind invalid texture!");

        glBindTexture(TextureTargetToGLEnum(target), texture.id);
    }

    void unbindTexture(TextureTarget target)
    {
        glBindTexture(TextureTargetToGLEnum(target), 0);
    }

    void uploadTextureData(const Texture texture)
    {
    };

    Framebuffer createFramebuffer()
    {
        uint id;
        glGenFramebuffers(1, &id);
        return { id };
    }

    void deleteFramebuffer(Framebuffer& framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer.id);
        framebuffer.id = 0;
    }

    void bindFramebuffer(const Framebuffer framebuffer, FramebufferOperation framebufferop)
    {
        glBindFramebuffer(FramebufferOperationToGLEnum(framebufferop), framebuffer.id);
    }

    ShaderProgram createShaderProgram(const char* name)
    {
        return {};
    }

    void deleteShaderProgram()
    {
    }

    void enableVertexLayout(VertexLayout layout)
    {
        switch (layout)
        {
        case VertexLayout::POINT:
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            break;
        case VertexLayout::POINT_UV:
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            break;
        case VertexLayout::TRIANGLE:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            break;
        case VertexLayout::TRIANGLE_UV:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            break;
        }
    }

    unsigned int compileShader(const char** data, const int* size, uint shader)
    {
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
            m_pos(pos), m_aspectRatio(aspectRatio)
    {
        update();
    }

    void Camera::update()
    {
        m_pos.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        m_pos.y = glm::sin(glm::radians(pitch));
        m_pos.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

        m_pos += m_dir * distance + offset;

        m_dir = glm::normalize(m_pos - target);
        m_right = glm::normalize(glm::cross({0.0f, 1.0f, 0.0f}, -m_dir));
        m_up = glm::cross(-m_dir, m_right);
        m_view = glm::lookAt(m_pos, target, m_up);
        m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_zNear, m_zFar);
    }

    /*
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
            RT_LOG_ERROR("Failed to draw to framebuffer");
        }
    }
    */

    std::optional<Texture> loadTexture(const char* filename)
    {
        int width, height;
        unsigned char* data = stbi_load(filename, &width, &height, nullptr, 4);

        if (!data)
        {
            RT_LOG_WARNING("Failed to load texture {}", filename);
            return std::nullopt;
        }

        // std::optional<Texture> tex(std::in_place, filename, data, width, height);
        stbi_image_free(data);
        RT_LOG_MSG("Loaded texture {} with size: {}x{}", filename, width, height);
        // return tex;
        return {};
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

    void setUniform(unsigned int shader, const char* name, glm::vec4 v)
    {
        int loc = glGetUniformLocation(shader, name);
        glUniform4f(loc, v[0], v[1], v[2], v[3]);
    }

    void setUniform(unsigned int shader, const char* name, glm::mat4 m)
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
        // Pointer trickery to avoid "error: casting loses precision"
        // We don't care about losing precision
        GLuint texID = *((unsigned int*)tex);
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
            RT_LOG_ERROR("---SHADER COMPILE LOG BEGIN---");
            RT_LOG_ERROR(infoLog);
            RT_LOG_ERROR("---SHADER COMPILE LOG END---");
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
            RT_LOG_ERROR("---SHADER LINK LOG BEGIN---");
            RT_LOG_ERROR(infoLog);
            RT_LOG_ERROR("---SHADER LINK LOG END---");
            return false;
        }
        return true;
    }

    void blitTexture(unsigned int target, int width, int height, const void* data)
    {
        glTexSubImage2D(target, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    bool toggleWireframe()
    {
        // Gl context is global so it's fine to use static
        static bool toggle = false;
        toggle ^= 1;
        if (toggle)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        return toggle;
    }

    int loadGLExtensions()
    {
        return glewInit();
    }

    std::string getGPUVendor()
    {
        return { reinterpret_cast<const char*>(glGetString(GL_VENDOR)) };
    }

    std::string getRenderer()
    {
        return { reinterpret_cast<const char*>(glGetString(GL_RENDERER)) };
    }

    std::string getGLVersion() 
    {
        return { reinterpret_cast<const char*>(glGetString(GL_VERSION)) };
    }

    std::string getGLSLVersion()
    {
        return { reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) };
    }
}