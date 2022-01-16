#ifndef RAYTRACER_EDITOR_H
#define RAYTRACER_EDITOR_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <cstddef>
#include <vector>
#include "Widgets.h"

// Example usage:
// First create a window:
//      Options options{};
//      auto window = Editor::createWindow("test", 800, 600, {});
// Then initialize the editor:
//      Editor::init(window, options);
// Then begin render/input loop
//      Editor::renderLoop(window);

namespace Editor
{
    struct Options
    {
        bool enableViewports         = false; // Viewports allow moving editor windows outside main window
        bool enableVsync             = true;
        bool enableMainWindowBorders = true;  // Enables OS window title bar and borders
    };

    // Creates GLFW window and loads OpenGL extensions
    GLFWwindow* createWindow(const char* title, int width, int height, const Options& options);
    void destroyWindow(GLFWwindow* window);

    // Creates editor context (Dear Imgui) and initializes it with options
    void init(GLFWwindow* window, const Options& options);

    // Begins executing draw/input loop
    void renderLoop(GLFWwindow* window, Widget* widgets, std::size_t numWidgets);

    // Stuff that is used internally
    void renderImguiDrawData();
    void renderGui(ImGuiIO &io, Widget* widgets, std::size_t numWidgets);
    void drawEditor(const ImGuiIO& io, Widget* widgets, std::size_t numWidgets);
    void windowErrorCallback(int code, const char* description);
    std::vector<Widget> createDefaultEditorWidgets();
}

#endif //RAYTRACER_EDITOR_H
