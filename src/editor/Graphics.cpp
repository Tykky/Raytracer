#include "Graphics.h"
#include "io/stbi.h"
#include <GL/glew.h>

namespace Editor
{
    bool loadGlTexture(const char* filename, GLtexture& gltexture)
    {
        unsigned char* data = stbi_load(filename, &gltexture.width, &gltexture.height, nullptr, 4);

        if (!data)
            return false;

        glGenTextures(1, &gltexture.textureID);
        glBindTexture(GL_TEXTURE_2D, gltexture.textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gltexture.width, gltexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        return true;
    }
}