#ifndef RAYTRACER_GRAPHICS_H
#define RAYTRACER_GRAPHICS_H

#include "GL/glew.h"
#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include "io/Io.h"

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

    // We clump OpenGls vertex buffer, vertex array and index (element)
    // buffer into one object
    class VertexBuffer
    {
    public:
        VertexBuffer(float* vertices, std::size_t size);
        ~VertexBuffer();
        VertexBuffer() = delete;
        VertexBuffer(const VertexBuffer& vertexBuffer) = delete;

        void bind();

    private:
        unsigned int m_vbo; // Vertex Buffer Object
        unsigned int m_vao; // Vertex Array Object
        unsigned int m_ebo; // Vertex Element Object
    };

    struct Framebuffer
    {
        Framebuffer();
        ~Framebuffer();

        unsigned int m_framebufferID;

        Framebuffer(const Framebuffer& framebuffer) = delete;
        Framebuffer& operator=(const Framebuffer& framebuffer) = delete;
    };

    // A texture we can use as render target
    class RenderTexture
    {
    public:
        RenderTexture(unsigned int width, unsigned int height, bool depthTesting);
        ~RenderTexture();
        RenderTexture() = delete;
        RenderTexture(const RenderTexture& renderTexture) = delete;
        RenderTexture &operator=(const RenderTexture& renderTexture) = delete;

    private:
        std::string m_name;
        int m_width;
        int m_height;
        unsigned int m_renderTextureId;
        unsigned int m_depthBufferId;
        const bool m_depthTestEnabled;
    };

    class ShaderProgram
    {
    public:
        ~ShaderProgram();
        ShaderProgram(const ShaderProgram& shaderProgram) = delete;
        ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;
        ShaderProgram(ShaderProgram&& shaderProgram);
        ShaderProgram& operator=(ShaderProgram&& shaderProgram);

        bool addShader(const char* path, ShaderType shaderType);
        bool link();

    private:
        unsigned int m_shaderProgramId   = 0;
        unsigned int m_vertexShaderId    = 0;
        unsigned int m_fragmentShaderId  = 0;
        unsigned int m_computeShaderId   = 0;
    };

    // Creates and compiles a shader
    unsigned int compileShader(const char** data, const int* size, ShaderType shaderType);

    // Simply uploads texture from disk
    // to GPU memory. Uses stb_image under the hood,
    // does have some restriction to what file formats
    // are supported.
    std::optional<Texture> loadTexture(const char* filename);

    void clear();

    // and this, use RAII instead
    void deleteTexture(unsigned int textureID);

    // Used by ImFileDialog
    void *createTexture(unsigned char* data, int w, int h, char fmt);
    void deleteTexture(void* tex);

    bool checkShaderCompilation(unsigned int shaderId);
    bool checkShaderLink(unsigned int shaderId);

    typedef std::vector<Editor::Texture> TextureStore;
}

#endif //RAYTRACER_GRAPHICS_H
