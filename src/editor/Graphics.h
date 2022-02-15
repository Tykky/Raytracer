#ifndef RAYTRACER_GRAPHICS_H
#define RAYTRACER_GRAPHICS_H

#include "GL/glew.h"
#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include "io/Io.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Quad for testing purposes

static constexpr float defaultRectangleData[] =
{
//       Vertices
//  <----------------->
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
};

// Cube for testing purposes
static constexpr float defaultCubeData[] =
{
//       Vertices           tex uv
//  <----------------->  <---------->
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

namespace Editor
{
    // Measures time in between frames
    float getDeltaTime();

    enum class ShaderType
    {
        VERTEX   = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        COMPUTE  = GL_COMPUTE_SHADER
    };

    // Wrapper for OpenGL texture.
    // TODO: use RAII
    struct Texture
    {
        std::string name;
        int width;
        int height;
        unsigned int textureID;
    };

    // We clump OpenGls vertex buffer, vertex array and element
    // buffer into one object
    class Vertexbuffer
    {
    public:
        Vertexbuffer() = delete;
        Vertexbuffer(const float* vertices, std::size_t size);
        ~Vertexbuffer();

        Vertexbuffer(const Vertexbuffer& vertexbuffer) = delete;
        Vertexbuffer& operator=(const Vertexbuffer& vertexbuffer) = delete;
        Vertexbuffer(Vertexbuffer&& vertexbuffer);
        Vertexbuffer& operator=(Vertexbuffer&& vertexbuffer);

        void bind();

    private:
        unsigned int m_vbo; // Vertex Buffer Object
        unsigned int m_vao; // Vertex Array Object
        // unsigned int m_ebo; // Vertex Element Object
    };

    // A texture we can use as render target
    class RenderTexture
    {
    public:
        RenderTexture() = default;
        RenderTexture(unsigned int width, unsigned int height, bool depthTesting);
        ~RenderTexture();

        RenderTexture(const RenderTexture& renderTexture) = delete;
        RenderTexture &operator=(const RenderTexture& renderTexture) = delete;
        RenderTexture(RenderTexture&& renderTexture);
        RenderTexture& operator=(RenderTexture&& renderTexture);

        inline unsigned int id() const { return m_renderTextureId; }
        inline unsigned int depthId() const { return m_depthRenderBufferId; }
        inline int getHeight() const { return m_height; }
        inline int getWidth() const { return m_width; }

    private:
        int          m_width               = 0;
        int          m_height              = 0;
        unsigned int m_renderTextureId     = 0;
        unsigned int m_depthRenderBufferId = 0;
        bool         m_isDepthTestEnabled  = 0;
    };

    class Framebuffer
    {
    public:
        Framebuffer();
        ~Framebuffer();

        Framebuffer(const Framebuffer& framebuffer) = delete;
        Framebuffer& operator=(const Framebuffer& framebuffer) = delete;
        Framebuffer(Framebuffer&& framebuffer);
        Framebuffer& operator=(Framebuffer&& framebuffer);

        void addColorAttachment(RenderTexture&& renderTexture);

        inline unsigned int getNumColorAttachments() const { return m_numColorAttachments; }
        inline const RenderTexture* getColorAttachments() const { return m_colorAttachments; }

        void bind();
        void unbind();
        void clear();

    private:
        unsigned int   m_framebufferID;
        RenderTexture  m_colorAttachments[16];
        unsigned int   m_numColorAttachments = 0;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram() = default;
        ~ShaderProgram();

        ShaderProgram(const ShaderProgram& shaderProgram) = delete;
        ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;
        ShaderProgram(ShaderProgram&& shaderProgram);
        ShaderProgram& operator=(ShaderProgram&& shaderProgram);

        bool addShader(const char* path, ShaderType shaderType);
        bool link();
        void use();

        inline unsigned int getProgramId() { return m_shaderProgramId; }

    private:
        unsigned int m_shaderProgramId   = 0;
        unsigned int m_vertexShaderId    = 0;
        unsigned int m_fragmentShaderId  = 0;
        unsigned int m_computeShaderId   = 0;
    };

    class Camera
    {
    public:
        Camera() = delete;
        Camera(float aspectRatio, glm::vec3 pos, glm::vec3 target);

        // Applies all changes done
        void update();

        glm::vec3 getDir() { return m_dir; }

        inline glm::mat4 getProjectionMatrix() { return m_projection; };
        inline glm::mat4 getViewMatrix() { return m_view; }
        inline glm::vec3 getRight() { return m_right; }

        float speed = 1.0f;
        glm::vec3 pos;
        float     yaw = -90.0f;
        float     pitch = 0.0f;

    private:
        float m_aspectRatio;

        glm::vec3 m_dir;
        glm::vec3 m_right;
        glm::vec3 m_up;
        float     m_fov   = 90.0f; // field of view in degrees
        float     m_zNear = 0.1f;
        float     m_zFar  = 100.0f;

        glm::mat4 m_view;
        glm::mat4 m_projection;
    };

    // Creates and compiles a shader
    unsigned int compileShader(const char** data, const int* size, ShaderType shaderType);

    // Draws all current color attachments
    void drawToTexture(Vertexbuffer& vertexBuffer, ShaderProgram& shader, Framebuffer& framebuffer);

    // Simply uploads texture from disk
    // to GPU memory. Uses stb_image under the hood,
    // does have some restriction to what file formats
    // are supported.
    std::optional<Texture> loadTexture(const char* filename);

    void clear();

    void setUniform(unsigned int shader, const char* name, glm::vec3 v);
    void setUniform(unsigned int shader, const char* name, glm::vec4 v);
    void setUniform(unsigned int shader, const char* name, glm::mat4 m);

    // and this, use RAII instead
    void deleteTexture(unsigned int textureID);

    // Used by ImFileDialog
    void* createTexture(unsigned char* data, int w, int h, char fmt);
    void deleteTexture(void* tex);

    bool checkShaderCompilation(unsigned int shaderId);
    bool checkShaderLink(unsigned int shaderId);

    typedef std::vector<Editor::Texture> TextureStore;
}

#endif //RAYTRACER_GRAPHICS_H