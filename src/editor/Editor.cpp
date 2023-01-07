#include "Editor.h"
#include "Widgets.h"
#include "editor/Graphics.h"
#include "editor/styles/DarkTheme.h"
// #include "ImFileDialog.h"
#include "util/Math.h"
#include <stdexcept>

namespace Editor 
{
    // Internal forward declaration
    void renderGui(ImGuiIO& io, EditorContext& ctx);
    void drawEditor(const ImGuiIO& io, EditorContext& ctx);
    void windowErrorCallback(int code, const char* description);
    void dragAndResizeFromEdges(EditorContext& ctx);
    void updateEditorContext(EditorContext& ctx);
    int  checkCursorEdge(v2d relPos, v2i32 windowSize, float resizeAreaSize, float dragAreaSize);
    void changeCursorOnEdge(GLFWwindow* win, int flag);

    void setWindowSize(GLFWwindow* window, v2i32 size, EditorContext& ctx);
    void setWindowPos(GLFWwindow* window, v2i32 pos, EditorContext& ctx);

    void updateCursorPosAndDelta(EditorContext& ctx);
    void updateWindowSize(EditorContext& ctx);
    void updateWindowPos(EditorContext& ctx);

    void maximizeWindow(EditorContext& ctx);
    void minimizeWindow(EditorContext& ctx);

    void createDefaultEditorWidgets(WidgetStore& widgetStore);
    void logVendorInfo();
    // std::vector<FilePath> filesInsideDirectory();

    void initEditor(EditorContext& ctx) 
    {
        initLogger(128);

        if (!glfwInit())
            throw std::runtime_error("Initializing GLFW failed");
			
        glfwSetErrorCallback(windowErrorCallback);

    	ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        if (ctx.options.enableViewports) 
        {
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        }

        io.ConfigWindowsMoveFromTitleBarOnly = true;
        execDarkTheme();

        /*
        // ImFIleDialog needs functions for allocating freeing textures for icons
        ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void*
        {
            return createTexture(data, w, h, fmt);
        };

        ifd::FileDialog::Instance().DeleteTexture = [](void* tex)
        {
            deleteTexture(tex);
        };
        */

        // Load custom font
        io.Fonts->AddFontFromFileTTF("data/fonts/Roboto-Regular.ttf", 16.0f);
        io.IniFilename = "data/config/default-settings.ini";

#ifdef NDEBUG
        // We don't want to save user settings for now, maybe at sometime there is a feature to
        // save and load user settings. For nowe we only load the default initEditor file.
        io.WantSaveIniSettings = false;
#endif

        // setMouseScrollCallback();
        ctx.initialized = true;
    }

    void createEditorWindow(const char* title, int width, int height, EditorContext& ctx)
    {
		if (!ctx.options.enableMainWindowBorders)
		{
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		}

		ctx.window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(ctx.window);

		if (!ctx.options.enableVsync)
		{
			RT_LOG_MSG("Vsync disabled");
			glfwSwapInterval(0);
		}
		else
		{
			RT_LOG_MSG("Vsync enabled");
		}

		logVendorInfo();
    }

    void destroyWindow(EditorContext& ctx)
    {
        if (!ctx.window)
        {
            RT_LOG_WARNING("Tried to destroy window when one hasn't been created yet!");
            return;
        }

        glfwDestroyWindow(ctx.window);
        RT_LOG_MSG("GLFW window destroyed");
        glfwTerminate();
        RT_LOG_MSG("GLFW terminated");
    }

    void initGui(EditorContext& ctx)
    {
        /*
        if (loadGLExtensions() != LOADGL_OK)
            throw std::exception("Failed to load GL extensions");
        */

        ImGuiImplInitGLFW(ctx.window);
        ImGuiImplInitGL3("#version 440");
        createDefaultEditorWidgets(ctx.widgetStore);
    }

    void renderLoop(EditorContext& ctx) 
    {
        if (!ctx.window)
        {
            RT_LOG_WARNING("Tried to enter renderloop when there is no valid window yet!");
            return;
        }

        if (!ctx.initialized)
        {
            RT_LOG_WARNING("Tried to enter render loop when the editor is not yet initialized!");
            return;
        }

        GLFWwindow* glfwWindow = ctx.window;
        ImGuiIO& io = ImGui::GetIO();
        double current = glfwGetTime();
        double last;
        while (!glfwWindowShouldClose(glfwWindow)) 
        {
            last = current;
            current = glfwGetTime();
            ctx.deltaTime = current - last;

            glfwPollEvents();
            // clear();
            renderGui(io, ctx);
            glfwSwapBuffers(glfwWindow);
        }
    }

    // Internal implementation

    void windowErrorCallback(int code, const char* description)
    {
        RT_LOG_ERROR("[GLFW CALLBACK] ({}) {}", code, description);
    }

    void updateEditorContext(EditorContext& ctx)
    {
        updateCursorPosAndDelta(ctx);
        updateWindowSize(ctx);
        updateWindowPos(ctx);
        if (!ctx.options.enableMainWindowBorders)
            dragAndResizeFromEdges(ctx);
    }

    void drawEditor(const ImGuiIO& io, EditorContext& ctx) 
    {
        drawMainMenuBar(ctx.widgetStore, ctx.options.enableMainWindowBorders);
        drawImFileDialogAndProcessInput();
        drawDockspace("Main");
        drawAllWidgets(&ctx.widgetStore);
        deleteAllMarkedWidgets(ctx.widgetStore);
    }

    void renderGui(ImGuiIO& io, EditorContext& ctx) 
    {
        ImGuiImplGLNewFrame();
        ImGuiImplGLFWNewFrame();
        ImGui::NewFrame();

        updateEditorContext(ctx);
        drawEditor(io, ctx);

        ImGui::Render(); // calls Imgui::EndFrame()

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
        {
            GLFWwindow* window = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(window);
        }
        ImGuiRenderDrawData();
    }

    typedef int ResizeFlag;

    constexpr ResizeFlag
        NONE          = 0,
        DRAG_TOP      = 1 << 0,
        RESIZE_TOP    = 1 << 1,
        RESIZE_RIGHT  = 1 << 2,
        RESIZE_BOTTOM = 1 << 3,
        RESIZE_LEFT   = 1 << 4;


    int checkCursorEdge(v2d relPos, v2i32 windowSize, float resizeAreaSize, float dragAreaSize)
    {
        int flag = NONE;

        if (relPos.y() < resizeAreaSize)
            flag |= RESIZE_TOP;

        if (relPos.y() >= resizeAreaSize && relPos.y() < dragAreaSize)
            flag |= DRAG_TOP;

        if (relPos.x() > windowSize.x() - resizeAreaSize)
            flag |= RESIZE_RIGHT;

        if (relPos.y() > windowSize.y() - resizeAreaSize)
            flag |= RESIZE_BOTTOM;

        if (relPos.x() < resizeAreaSize)
            flag |= RESIZE_LEFT;

        return flag;
    }

    void changeCursorOnEdge(GLFWwindow* win, int flag)
    {
        static GLFWcursor* resizeHorisontalCursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        static GLFWcursor* resizeVerticalCursor   = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        static GLFWcursor* resizeNESWCursor       = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
        static GLFWcursor* resizeNWSECursor       = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);

        int vertFlag = RESIZE_TOP   | RESIZE_BOTTOM;
        int horFlag  = RESIZE_LEFT  | RESIZE_RIGHT;
        int NEFlag   = RESIZE_RIGHT | RESIZE_TOP;
        int NWFlag   = RESIZE_LEFT  | RESIZE_TOP;
        int SEFlag   = RESIZE_RIGHT | RESIZE_BOTTOM;
        int SWFlag   = RESIZE_LEFT  | RESIZE_BOTTOM;

		if (flag == NEFlag || flag == SWFlag)
            glfwSetCursor(win, resizeNESWCursor);

        else if (flag == NWFlag || flag == SEFlag)
            glfwSetCursor(win, resizeNWSECursor);

        else if (flag & horFlag)
            glfwSetCursor(win, resizeHorisontalCursor);

        else if (flag & vertFlag)
            glfwSetCursor(win, resizeVerticalCursor);
    }

    void setWindowSize(GLFWwindow* window, v2i32 size, EditorContext& ctx)
    {
        glfwSetWindowSize(window, size.x(), size.y());
        ctx.windowSize = size;
    }

    void setWindowPos(GLFWwindow* window, v2i32 pos, EditorContext& ctx)
    {
        glfwSetWindowPos(window, pos.x(), pos.y());
        ctx.windowPos = pos;
    }

    // hold contains positions where the mouse was clicked first time
    // flag contains position where the mouse is hovered
    // check contains the positions to be checked for e.g RESIZE_TOP, RESIZE_LEFT etc..., see definition of ResizeFlag
    void executeWhileMouse1Pressed(GLFWwindow* win, ResizeFlag& hold, const ResizeFlag flag, const ResizeFlag check, EditorContext& ctx, void (*exec)(GLFWwindow* win, EditorContext& ctx))
    {
        /*
        if ((hold & check) || (getMouseButton(MouseCode::MOUSE_BUTTON_1) == StatusCode::PRESS && flag & check))
        {
            hold |= check;
            exec(win, ctx);
            if (getMouseButton(MouseCode::MOUSE_BUTTON_1) == StatusCode::RELEASE)
                hold &= ~check;
        }
        */
    }

	void dragAndResizeFromEdges(EditorContext& ctx)
    {
        // both in pixels
        constexpr double dragAreaSize   = 20.0f;
        constexpr double resizeAreaSize = 5.0f;

        GLFWwindow* win = glfwGetCurrentContext();

        int flag = checkCursorEdge(ctx.cursorRelativePos, ctx.windowSize, resizeAreaSize, dragAreaSize);
        static int hold = NONE; // to allow dragging/scaling when mouse goes slightly over the edge

        changeCursorOnEdge(win, flag);

        // Drag from top
        if (!(hold & RESIZE_TOP)) // do not resize when window is dragged
            executeWhileMouse1Pressed(win, hold, flag, DRAG_TOP, ctx, [](GLFWwindow* win, EditorContext& ctx)
            {
                setWindowPos(
                        win, {
                        ctx.windowPos.x() + static_cast<int>(ctx.cursorDelta.x()),
                        ctx.windowPos.y() + static_cast<int>(ctx.cursorDelta.y())},
                        ctx);
            });

        // Resize from top
        if (!(hold & DRAG_TOP)) // do not drag when window is being resized from top
            executeWhileMouse1Pressed(win, hold, flag, RESIZE_TOP, ctx, [](GLFWwindow *win, EditorContext& ctx)
            {
                setWindowSize(
                        win, {
                        ctx.windowSize.x(),
                        ctx.windowSize.y() - static_cast<int>(ctx.cursorDelta.y())},
                        ctx);

                setWindowPos(
                        win, {
                        ctx.windowPos.x(),
                        ctx.windowPos.y() + static_cast<int>(ctx.cursorDelta.y())},
                        ctx);
            });

        // Resize from left
        executeWhileMouse1Pressed(win, hold ,flag, RESIZE_LEFT, ctx, [](GLFWwindow* win, EditorContext& ctx)
        {
            setWindowSize(
                    win, {
                    ctx.windowSize.x() - static_cast<int>(ctx.cursorDelta.x()),
                    ctx.windowSize.y()},
                    ctx);

            setWindowPos(
                    win, {
                    ctx.windowPos.x() + static_cast<int>(ctx.cursorDelta.x()),
                    ctx.windowPos.y()},
                    ctx);
        });

        // Resize from right
        executeWhileMouse1Pressed(win, hold, flag, RESIZE_RIGHT, ctx, [](GLFWwindow* win, EditorContext& ctx)
        {
            setWindowSize(
                    win, {
                    ctx.windowSize.x() + static_cast<int>(ctx.cursorDelta.x()),
                    ctx.windowSize.y()},
					ctx);
        });

        // Resize from bottom
        executeWhileMouse1Pressed(win, hold, flag, RESIZE_BOTTOM, ctx, [](GLFWwindow* win, EditorContext& ctx)
        {
            setWindowSize(
                    win, {
                    ctx.windowSize.x(),
                    ctx.windowSize.y() + static_cast<int>(ctx.cursorDelta.y())},
                    ctx);
        });
	}

    void updateWindowSize(EditorContext& ctx)
   {
        int w, h;
        glfwGetWindowSize(ctx.window, &w, &h);
        ctx.windowSize = { w, h };
    }

    void updateWindowPos(EditorContext& ctx)
    {
        int w, h;
        glfwGetWindowPos(ctx.window, &w, &h);
        ctx.windowPos = {w, h};
    }

    void updateCursorPosAndDelta(EditorContext& ctx)
    {
        double mx, my; // mouse position
        int wx, wy;    // Window position
        glfwGetCursorPos(ctx.window, &mx, &my);
        glfwGetWindowPos(ctx.window, &wx, &wy);

        ctx.cursorDelta.x() = mx + wx - ctx.cursorPos.x();
        ctx.cursorDelta.y() = my + wy - ctx.cursorPos.y();

        // Relative to screen
        ctx.cursorPos = { mx + wx, my + wy};

        // Relative to window position
        ctx.cursorRelativePos = { mx, my };
    }

    void createDefaultEditorWidgets(WidgetStore& widgetStore)
    {
        insertWidgetArray<ViewportContext>(widgetStore, "Viewport context");
        insertWidgetArray<LogViewerContext>(widgetStore, "Log viewer");
        insertWidgetArray<RTControlsContext>(widgetStore, "RT Controls");
        insertWidgetArray<SystemInfoContext>(widgetStore, "System info");
        insertWidgetArray<RasterSettingsContext>(widgetStore, "Raster settings");
    }


    /*
    std::vector<FilePath> filesInsideDirectory()
    {
        return {};
    }
    */

    void logVendorInfo()
    {
        /*
        RT_LOG_MSG("vendor: {}", getGPUVendor());
        RT_LOG_MSG("renderer: {}", getRenderer());
        RT_LOG_MSG("GL version: {}", getGLVersion());
        RT_LOG_MSG("GLSL version: {}", getGLSLVersion());
        */
    }

    void maximizeWindow(EditorContext& ctx)
    {
        if (!ctx.windowMaximized)
        {
            glfwMaximizeWindow(ctx.window);
        }
        else
        {
            glfwRestoreWindow(ctx.window);
        }
        ctx.windowMaximized ^= 1;
    }

    void minimizeWindow(EditorContext& ctx)
    {
        RT_LOG_MSG("Window minimzed");
        glfwIconifyWindow(ctx.window);
    }
}
