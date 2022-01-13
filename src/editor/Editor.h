#ifndef RAYTRACER_EDITOR_H
#define RAYTRACER_EDITOR_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <cstddef>

namespace Editor
{
    struct Options
    {
        bool enableViewports = false;
        bool enableVsync     = true;
    };

    void init(GLFWwindow* window, const Options& options);
    GLFWwindow* createWindow(int width, int height, const char* title);
    void destroyWindow(GLFWwindow* window);
    void windowErrorCallback(int code, const char* description);

    void renderGui(ImGuiIO& io);
    void renderLoop(GLFWwindow* window);
    void renderImguiDrawData();
}

#endif //RAYTRACER_EDITOR_H
