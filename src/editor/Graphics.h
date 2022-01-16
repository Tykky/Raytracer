#ifndef RAYTRACER_GRAPHICS_H
#define RAYTRACER_GRAPHICS_H

#include <vector>

// Wrapper for OpenGL texture.
// The texture should be already in the
// GPU memory when this gets passed around
struct GLtexture
{
    int width;
    int height;
    unsigned int textureID;
};

// Simply uploads texture from disk
// to GPU memory. Uses stb_image under the hood,
// does have some restriction to what file formats
// are supported etc...
bool loadGlTexture(const char* filename);

#endif //RAYTRACER_GRAPHICS_H
