#ifndef RAYTRACER_WINDOW_H
#define RAYTRACER_WINDOW_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "core/Raytracer.h"

class Window {

private:
    std::shared_ptr<Raytracer> raytracer;
    GLFWwindow *window;
    int width;
    int height;
    void render() const;
    /** @return texture id */
    unsigned int setupTexture() const;
    /** @param size is size of vertices array in bytes */
    unsigned int setupVertexBufferObject(const float vertices[], const unsigned int &size) const;
    unsigned int setupVertexArrayObject() const;

    /**
     * @brief Creates new GLSL shader and compiles it
     * @param source contains GLSL shader source code
     * @param shadertype, choose GL_VERTEX_SHADER, GL_FAGMENT_SHADER etc..
     */
    unsigned int setupShader(const char *source, const unsigned int &type) const;

    /** 
     * @param shaders is a array of already compiled shaders
     * @param size is number of elements in the shaders[] array
     */
    void linkShaders(const int &shaderprogram, unsigned int shaders[], const unsigned int &size) const;

public:
    Window(int width, int height, const char *title, std::shared_ptr<Raytracer> raytracer);
    ~Window();
};


#endif //RAYTRACER_WINDOW_H
