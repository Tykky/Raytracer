#ifndef RAYTRACER_EDITOR_H
#define RAYTRACER_EDITOR_H

#include "logging/Logging.h"
#include "util/Types.h"
#include <GLFW/glfw3.h>
#include "Widgets.h"

// Everything related to resource management for the Editor.
// Contains the main functions to create and destroy the editor context.

namespace Editor
{
	#ifdef WIN32
		static const char* FONT_PATH = "C:\\Windows\\Fonts";
	#elif defined(__linux__)
		static const char* FONT_PATH = "/usr/share/fonts";
	#else
		static const char* FONT_PATH = nullptr;
	#endif

    // Editor API declaration
    struct Options
    {
        bool enableViewports         = false; // Viewports allow moving editor windows outside the main window
        bool enableVsync             = true;
        bool enableMainWindowBorders = true;  // Enables OS window title bar and borders
        std::string defaultTheme     = "dark";
    };

    struct EditorContext 
    {
        Options      options;
        GLFWwindow*  window             = nullptr;
        float        deltaTime          = 0.0f;
        float        fps                = 0.0f;
        float        mouseScroll        = 0.0f;
        v2d          cursorPos          = { 0.0f, 0.0f }; // Screen coordinates
        v2d          cursorRelativePos  = { 0.0f, 0.0f }; // Relative to window position
        v2d          cursorDelta        = { 0.0f, 0.0f };
        v2i32        windowSize         = { 0, 0 };
        v2i32        windowPos          = { 0, 0};
        WidgetStore  widgetStore;
        bool         initialized        = false;
        bool         windowMaximized    = false;
    };

    // Creates editor context
    void initEditor(EditorContext& ctx);

    void createEditorWindow(const char* title, int width, int height, EditorContext& ctx);
    void destroyWindow(EditorContext& ctx);

    void initGui(EditorContext& ctx);

    // Begins executing draw loop
    void renderLoop(EditorContext& ctx);
}

#endif //RAYTRACER_EDITOR_H
