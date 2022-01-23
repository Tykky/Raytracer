#ifndef RAYTRACER_GRAPHICS_H
#define RAYTRACER_GRAPHICS_H

#include <vector>
#include <unordered_map>
#include <optional>
#include <string>

namespace Editor::Gfx
{
    // Wrapper for OpenGL texture.
    struct GLtexture
    {
        std::string   name;
        int           width;
        int           height;
        unsigned int  textureID;
    };

    // Simply uploads texture from disk
    // to GPU memory. Uses stb_image under the hood,
    // does have some restriction to what file formats
    // are supported.
    std::optional<GLtexture> loadTexture(const char* filename);

   void clear();
   void deleteTexture(unsigned int textureID);
}

namespace Editor
{
    typedef std::vector<Editor::Gfx::GLtexture> TextureStore;
}

#endif //RAYTRACER_GRAPHICS_H
