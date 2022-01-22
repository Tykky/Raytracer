#include "Graphics.h"
#include "io/stbi.h"
#include "logging/Logging.h"
#include <GL/glew.h>
#include <stdexcept>
#include <string>

namespace Editor::Gfx
{
    std::optional<GLtexture> loadTexture(const char* filename, MessageStore& messageStore)
    {
        GLtexture gltexture;
        unsigned char* data = stbi_load(filename, &gltexture.width, &gltexture.height, nullptr, 4);

        if (!data)
        {
            std::string msg = "Failed to load texture " + std::string(filename);
            // logWarning(msg.data());
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

        std::string msg = "Loaded texture " + std::string(filename);
        logMsg(std::move(msg), messageStore);

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
}