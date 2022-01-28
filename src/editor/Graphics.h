#ifndef RAYTRACER_GRAPHICS_H
#define RAYTRACER_GRAPHICS_H

#include <vector>
#include <unordered_map>
#include <optional>
#include <string>

namespace Editor::Gfx
{
    // Wrapper for OpenGL texture.
    // TODO: use RAII
    struct Texture
    {
        std::string   name;
        int           width;
        int           height;
        unsigned int  textureID;
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
        RenderTexture& operator=(const RenderTexture& renderTexture) = delete;

    private:
        std::string   m_name;
        int           m_width;
        int           m_height;
        unsigned int  m_renderTextureId;
        unsigned int  m_depthBufferId;
        const bool    m_depthTestEnabled;
    };

    // Simply uploads texture from disk
    // to GPU memory. Uses stb_image under the hood,
    // does have some restriction to what file formats
    // are supported.
    std::optional<Texture> loadTexture(const char* filename);
    void* createTexture(unsigned char* data, int w, int h, char fmt);

   void clear();
   // and this, use RAII instead
   void deleteTexture(unsigned int textureID);

   // Used by ImFileDialog
   void* createTexture(unsigned char* data, int w, int h, char fmt);
   void deleteTexture(void* tex);

}

namespace Editor
{
    typedef std::vector<Editor::Gfx::Texture> TextureStore;
}

#endif //RAYTRACER_GRAPHICS_H
