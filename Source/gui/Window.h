#ifndef RAYTRACER_WINDOW_H
#define RAYTRACER_WINDOW_H

#define GLEW_STATIC
#include "GL/glew.h"
#include <GLFW/glfw3.h>

class Window {

private:
    GLFWwindow *window;
    void render() const;
    void setupTexture() const;

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
    void linkShaders(const int &shaderprogram,int shaders[], const unsigned int &size) const;

public:
    Window(int width, int height, const char *title);
    ~Window();
};


#endif //RAYTRACER_WINDOW_H
