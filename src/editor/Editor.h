#ifndef RAYTRACER_EDITOR_H
#define RAYTRACER_EDITOR_H

#include "logging/Logging.h"
#include "editor/Types.h"

namespace Editor
{
    //------------------------//
    // Editor API declaration //
    //------------------------//

    struct Options
    {
        bool enableViewports         = false; // Viewports allow moving editor windows outside the main window
        bool enableVsync             = true;
        bool enableMainWindowBorders = true;  // Enables OS window title bar and borders
        const char* defaultTheme     = "dark";
    };

    // Creates GLFW window and loads OpenGL extensions
    void* createWindow(const char* title, int width, int height, const Options& options);
    void destroyWindow(void* window);

    // Creates editor context (Dear Imgui) and initializes it with options
    void init(void* window, const Options& options);

    // Begins executing draw/input loop
    void renderLoop(void* window);
}

#endif //RAYTRACER_EDITOR_H