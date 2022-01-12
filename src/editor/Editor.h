#ifndef RAYTRACER_EDITOR_H
#define RAYTRACER_EDITOR_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace editor
{
    void initEditor(GLFWwindow* window);
    GLFWwindow* createWindow(int width, int height, const char* title);
    void loadGl();
    void windowErrorCallback(int code, const char* description);
    void renderLoop(GLFWwindow* window, const char* description);
}

#endif //RAYTRACER_EDITOR_H
