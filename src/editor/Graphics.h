#ifndef RAYTRACER_GRAPHICS_H
#define RAYTRACER_GRAPHICS_H

#include "GL/glew.h"
#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include "io/Io.h"

namespace Editor
{
    enum class ShaderType
    {
        VERTEX = GL_VERTEX_SHADER,
        TESS_CONTROL = GL_TESS_CONTROL_SHADER,
        TESS_EVAL = GL_TESS_EVALUATION_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        COMPUTE = GL_COMPUTE_SHADER
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

    class Framebuffer
    {
    public:
        Framebuffer();
        ~Framebuffer();
    private:
        unsigned int m_framebufferID;
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

    class Shader
    {
    public:
        Shader(const char* name, const char* filePath, ShaderType shaderType);
        ~Shader();

        Shader() = delete;
        Shader(const Shader& shader) = delete;
        Shader& operator=(const Shader& shader) = delete;
        Shader(Shader&& shader);
        Shader& operator=(Shader&& shader);

        inline unsigned int getId() { return m_shaderId; }
        inline const std::string& getName() const { return m_name; }
        bool compile();

    private:
        std::string  m_name;
        std::string  m_filePath;
        unsigned int m_shaderId;
        ShaderType   m_shaderType;
        // Using these to avoid using exceptions
        bool         m_readFileSuccess = false;
        bool         m_compileSuccess  = false;
    };

    typedef std::vector<Shader> ShaderStore;

    class ShaderProgram
    {
    public:
        ShaderProgram(ShaderStore* shaders);
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram& shaderProgram) = delete;
        ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;
    private:
        ShaderStore* m_shaders;
        unsigned int m_shaderProgramId;
        // to avoid exceptions
        bool         m_programSuccess = false;
    };

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

}
namespace Editor
{
    typedef std::vector<Editor::Texture> TextureStore;
}

#endif //RAYTRACER_GRAPHICS_H
