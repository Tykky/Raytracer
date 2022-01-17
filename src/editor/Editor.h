#ifndef RAYTRACER_EDITOR_H
#define RAYTRACER_EDITOR_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <cstddef>
#include <vector>
#include "Widgets.h"

namespace Editor
{
    struct Options
    {
        bool enableViewports         = false; // Viewports allow moving editor windows outside the main window
        bool enableVsync             = true;
        bool enableMainWindowBorders = true;  // Enables OS window title bar and borders
    };

    // Creates GLFW window and loads OpenGL extensions
    GLFWwindow* createWindow(const char* title, int width, int height, const Options& options);
    void destroyWindow(GLFWwindow* window);

    // Creates editor context (Dear Imgui) and initializes it with options
    void init(GLFWwindow* window, const Options& options);

    // Begins executing draw/input loop
    void renderLoop(GLFWwindow* window);

    // Stuff that is used internally
    void renderImguiDrawData();
    void renderGui(ImGuiIO &io);
    void drawEditor(const ImGuiIO& io);
    void windowErrorCallback(int code, const char* description);
    void createDefaultEditorWidgets();
}

#endif //RAYTRACER_EDITOR_H
