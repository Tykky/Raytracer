#ifndef RAYTRACER_EDITOR_H
#define RAYTRACER_EDITOR_H

#include "logging/Logging.h"
#include "editor/Types.h"

// Everything related to resource managemnt for the Editor.
// Contains the main functions to create and destroy the editor context.

namespace Editor
{
    // Editor API declaration

    struct Options
    {
        bool enableViewports         = false; // Viewports allow moving editor windows outside the main window
        bool enableVsync             = true;
        bool enableMainWindowBorders = true;  // Enables OS window title bar and borders
        std::string defaultTheme     = "dark";
    };

    // Creates GLFW window and loads OpenGL extensions
    void createWindow(const char* title, int width, int height, const Options& options);
    void destroyWindow();

    // Creates editor context (Dear Imgui) and initializes it with options
    void init(const Options& options);

    // Begins executing draw/input loop
    void renderLoop();
}

#endif //RAYTRACER_EDITOR_H