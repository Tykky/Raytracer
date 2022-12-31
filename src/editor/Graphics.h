#ifndef RAYTRACER_GRAPHICS_H
#define RAYTRACER_GRAPHICS_H

#include <optional>
#include <string>
#include "io/Io.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "util/Types.h"

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
//  <----------------->   <--------->
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
    // Forward declaration of functions defined in Editor.cpp
    // This prevents circular dependency

    float getDeltaTime();

    // Graphics API declaration

    enum class ShaderType
    {
        VERTEX,
		FRAGMENT,
        COMPUTE
	};

    struct Texture
    {
        unsigned int id;
    };

    struct Framebuffer
    {
        unsigned int id;
    };

    struct ShaderProgram
    {
        unsigned int id;
    };

    Texture createTexture();

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
        inline glm::vec3 getUp() { return m_up; }

        glm::vec3 offset = { 0.0f, 0.0f, 0.0f };
        float     speed = 0.5f;
        glm::vec3 target = {0.0f, 0.0f, 0.0f};
        float     distance = 1.0f;
        float     yaw = 0.0f;
        float     pitch = 0.0f;

    private:
        float m_aspectRatio;

        glm::vec3 m_pos;
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

    // Draws to all current color attachments
    void drawToTexture(Vertexbuffer& vertexBuffer, ShaderProgram& shader, Framebuffer& framebuffer);

    // Simply uploads texture from disk
    // to GPU memory. Uses stb_image under the hood,
    // does have some restriction to what file formats are supported
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

    void blitTexture(unsigned int target, int width, int height, const void* data);

    // true: wireframe on, false: wireframe off
    bool toggleWireframe();

#define LOADGL_OK 0

    // Returns 0 when loading extensions is successful
    int loadGLExtensions();

    std::string getGPUVendor();
    std::string getRenderer();
    std::string getGLVersion();
    std::string getGLSLVersion();

    typedef std::vector<Editor::Texture> TextureStore;
}

#endif //RAYTRACER_GRAPHICS_H