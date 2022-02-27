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
        const char* defaultTheme     = "light";
    };

    // Creates GLFW window and loads OpenGL extensions
    GLFWwindow* createWindow(const char* title, int width, int height, const Options& options);
    void destroyWindow(GLFWwindow* window);

    // Creates editor context (Dear Imgui) and initializes it with options
    void init(GLFWwindow* window, const Options& options, WidgetStore& widgetStore, TextureStore& textureStore);

    // Begins executing draw/input loop
    void renderLoop(GLFWwindow* window, WidgetStore& widgetStore, TextureStore& textureStore);

    // Stuff that is used internally
    void renderImguiDrawData();
    void renderGui(ImGuiIO &io, WidgetStore& widgetStore, TextureStore& textureStore);
    void drawEditor(const ImGuiIO& io, const WidgetStore& widgetStore, TextureStore& textureStore);
    void windowErrorCallback(int code, const char* description);
    void createDefaultEditorWidgets(WidgetStore& widgets, TextureStore& textureStore);
}

#endif //RAYTRACER_EDITOR_H