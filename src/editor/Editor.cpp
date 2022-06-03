#include "Editor.h"
#include <GLFW/glfw3.h>
#include "Widgets.h"
#include "editor/Graphics.h"
#include "editor/styles/DarkTheme.h"
#include "editor/Input.h"
#include "core/Raytracer.h"
#include <map>

namespace Editor 
{
    // Internal forward declaration

    void renderGui(ImGuiIO& io);
    void drawEditor(const ImGuiIO& io);
    void windowErrorCallback(int code, const char* description);
    void dragAndResizeFromEdges();
    void updateEditorContext();
    int  checkCursorEdge(Vec2d relPos, Vec2i windowSize, float resizeAreaSize, float dragAreaSize);
    void changeCursorOnEdge(GLFWwindow* win, int flag);

    void setWindowSize(GLFWwindow* window, Vec2i size);
    void setWindowPos(GLFWwindow* window, Vec2i pos);

    void updateCursorPosAndDelta();
    void updateFps();
    void updateWindowSize();
    void updateWindowPos();

    float getDeltaTime();
    float getFps();
    GLFWwindow* getCurrentWindowHandle();
    Vec2i getWindowSize();

    void maximizeWindow();
    void minimizeWindow();

    void createDefaultEditorWidgets(WidgetStore& widgetStore);
    void logVendorInfo();
    std::vector<FilePath> filesInsideDirectory();

    // Editor Context

    struct EditorContext 
    {
        GLFWwindow*  window             = nullptr;
        float        deltaTime          = 0.0f;
        float        fps                = 0.0f;
        float        mouseScroll        = 0.0f;
        Vec2d        cursorPos          = { 0.0f, 0.0f }; // Screen coordinates
        Vec2d        cursorRelativePos  = { 0.0f, 0.0f }; // Relative to window position
        Vec2d        cursorDelta        = { 0.0f, 0.0f };
        Vec2i        windowSize         = { 0, 0 };
        Vec2i        windowPos          = {0, 0};
        WidgetStore  widgetStore;
        TextureStore textureStore;
        Raytracer    raytracer;
        bool         initialized        = false;
        bool         windowMaximized    = false;
    };

    // For now there is only one context
    static EditorContext context;

    EditorContext* ctx()
    {
        return &context;
    }

    // Editor API implementation

    void init(const Options& options) 
    {
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        if (options.enableViewports) 
        {
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        }

        io.ConfigWindowsMoveFromTitleBarOnly = true;
        execDarkTheme(); // use dark theme by default

        ImGuiImplInitGLFW(ctx()->window);
        ImGuiImplInitGL3("#version 440");

        // ImFIleDialog needs functions for creating and freeing textures for icons
        ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* 
        {
            return createTexture(data, w, h, fmt);
        };

        ifd::FileDialog::Instance().DeleteTexture = [](void* tex) 
        {
            deleteTexture(tex);
        };

        // Load custom font
        //io.Fonts->AddFontFromFileTTF("Roboto-Regular.ttf", 16.0f);
        io.IniFilename = "data/config/default-settings.ini";

#ifdef NDEBUG
        // We don't want to save user settings for now, maybe at sometime there is a feature to
        // save and load user settings. For nowe we only load the default init file.
        io.WantSaveIniSettings = false;
#endif

        createDefaultEditorWidgets(ctx()->widgetStore);
        setMouseScrollCallback();
        ctx()->initialized = true;
    }

    void createWindow(const char* title, int width, int height, const Options& options) 
    {
        if (ctx()->window)
        {
            RT_LOG_WARNING("Tried to create a window when there is already one!");
            return;
        }

        glfwSetErrorCallback(windowErrorCallback);
        GLFWwindow* window;
        if (glfwInit()) 
        {
            RT_LOG_MSG("GLFW initialized");
            if (!options.enableMainWindowBorders) 
            {
                glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            }

            window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwMakeContextCurrent(window);
            RT_LOG_MSG("GLFW window created");

            if (!options.enableVsync) 
            {
                RT_LOG_MSG("Vsync disabled");
                glfwSwapInterval(0);
            } 
            else 
            {
                RT_LOG_MSG("Vsync enabled");
            }

            if (loadGLExtensions() != LOADGL_OK) 
            {
                RT_LOG_ERROR("Failed to load GL extensions!");
                abort();
            }
            else 
            {
                RT_LOG_MSG("GL extensions loaded");
                logVendorInfo();
            }
        }
        else 
        {
            RT_LOG_ERROR("Failed to initialize GLEW");
            abort();
        }
        ctx()->window = window;
    }

    void destroyWindow() 
    {
        if (!ctx()->window)
        {
            RT_LOG_WARNING("Tried to destroy window when one hasn't been created yet!");
            return;
        }

        glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(ctx()->window));
        RT_LOG_MSG("GLFW window destroyed");
        glfwTerminate();
        RT_LOG_MSG("GLFW terminated");
    }

    void renderLoop() 
    {
        if (!ctx()->window)
        {
            RT_LOG_WARNING("Tried to enter renderloop when there is no valid window yet!");
            return;
        }

        if (!ctx()->initialized)
        {
            RT_LOG_WARNING("Tried to enter render loop when the editor is not initialized!");
            return;
        }

        GLFWwindow* glfwWindow = reinterpret_cast<GLFWwindow*>(ctx()->window);
        ImGuiIO& io = ImGui::GetIO();
        double current = glfwGetTime();
        double last;
        while (!glfwWindowShouldClose(glfwWindow)) 
        {
            last = current;
            current = glfwGetTime();
            ctx()->deltaTime = current - last;

            glfwPollEvents();
            clear();
            renderGui(io);
            glfwSwapBuffers(glfwWindow);
        }
    }

    // Internal implementation

    void windowErrorCallback(int code, const char* description)
    {
        RT_LOG_ERROR("[GLFW CALLBACK] ({}) {}", code, description);
    }

    void updateEditorContext()
    {
		updateFps();
        updateCursorPosAndDelta();
        updateWindowSize();
        updateWindowPos();
        dragAndResizeFromEdges();
    }

    void drawEditor(const ImGuiIO& io) 
    {
        drawMainMenuBar(ctx()->widgetStore, ctx()->textureStore);
        drawImFileDialogAndProcessInput();
        drawDockspace("Main");
        drawAllWidgets(&ctx()->widgetStore);
        deleteAllMarkedWidgets(ctx()->widgetStore);
    }

    void renderGui(ImGuiIO& io) 
    {
        ImGuiImplGLNewFrame();
        ImGuiImplGLFWNewFrame();
        ImGui::NewFrame();

        updateEditorContext();
        drawEditor(io);

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


    int checkCursorEdge(Vec2d relPos, Vec2i windowSize, float resizeAreaSize, float dragAreaSize)
    {
        int flag = NONE;

        if (relPos.y < resizeAreaSize)
            flag |= RESIZE_TOP;

        if (relPos.y >= resizeAreaSize && relPos.y < dragAreaSize)
            flag |= DRAG_TOP;

        if (relPos.x > windowSize.x - resizeAreaSize)
            flag |= RESIZE_RIGHT;

        if (relPos.y > windowSize.y - resizeAreaSize)
            flag |= RESIZE_BOTTOM;

        if (relPos.x < resizeAreaSize)
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

    void setWindowSize(GLFWwindow* window, Vec2i size)
    {
        glfwSetWindowSize(window, size.x, size.y);
        ctx()->windowSize = size;
    }

    void setWindowPos(GLFWwindow* window, Vec2i pos)
    {
        glfwSetWindowPos(window, pos.x, pos.y);
        ctx()->windowPos = pos;
    }

    // hold contains positions where the mouse was clicked first time
    // flag contains position where the mouse is hovered
    // check contains the positions to be checked for e.g RESIZE_TOP, RESIZE_LEFT etc..., see definition of ResizeFlag
    // TODO: come up with better name for this
    void executeWhileMouse1Pressed(GLFWwindow* win, ResizeFlag& hold, const ResizeFlag flag, const ResizeFlag check, void (*exec)(GLFWwindow* win))
    {
        if ((hold & check) || (getMouseButton(MouseCode::MOUSE_BUTTON_1) == StatusCode::PRESS && flag & check))
        {
            hold |= check;
            exec(win);
            if (getMouseButton(MouseCode::MOUSE_BUTTON_1) == StatusCode::RELEASE)
                hold &= ~check;
        }
    }

	void dragAndResizeFromEdges()
    {
        // both in pixels
        constexpr double dragAreaSize   = 20.0f;
        constexpr double resizeAreaSize = 5.0f;

        GLFWwindow* win = glfwGetCurrentContext();

        int flag = checkCursorEdge(ctx()->cursorRelativePos, ctx()->windowSize, resizeAreaSize, dragAreaSize);
        static int hold = NONE; // to allow dragging/scaling when mouse goes slightly over the edge

        changeCursorOnEdge(win, flag);

        // Drag from top
        if (!(hold & RESIZE_TOP)) // do not resize when window is dragged
            executeWhileMouse1Pressed(win, hold, flag, DRAG_TOP, [](GLFWwindow *win)
            {
                setWindowPos(
                        win,{
                        ctx()->windowPos.x + static_cast<int>(ctx()->cursorDelta.x),
                        ctx()->windowPos.y + static_cast<int>(ctx()->cursorDelta.y)});
                if (ctx()->windowMaximized)
                    maximizeWindow();
            });

        // Resize from top
        if (!(hold & DRAG_TOP)) // do not drag when window is being resized from top
            executeWhileMouse1Pressed(win, hold, flag, RESIZE_TOP, [](GLFWwindow *win)
            {
                setWindowSize(
                        win, {
                        ctx()->windowSize.x,
                        ctx()->windowSize.y - static_cast<int>(ctx()->cursorDelta.y)});

                setWindowPos(
                        win, {
                        ctx()->windowPos.x,
                        ctx()->windowPos.y + static_cast<int>(ctx()->cursorDelta.y)});
            });

        // Resize from left
        executeWhileMouse1Pressed(win, hold ,flag, RESIZE_LEFT, [](GLFWwindow* win)
        {
            setWindowSize(
                    win, {
                    ctx()->windowSize.x - static_cast<int>(ctx()->cursorDelta.x),
                    ctx()->windowSize.y});

            setWindowPos(
                    win, {
                    ctx()->windowPos.x + static_cast<int>(ctx()->cursorDelta.x),
                    ctx()->windowPos.y});
        });

        // Resize from right
        executeWhileMouse1Pressed(win, hold, flag, RESIZE_RIGHT, [](GLFWwindow* win)
        {
            setWindowSize(
                    win, {
                    ctx()->windowSize.x + static_cast<int>(ctx()->cursorDelta.x),
                    ctx()->windowSize.y});
        });

        // Resize from bottom
        executeWhileMouse1Pressed(win, hold, flag, RESIZE_BOTTOM, [](GLFWwindow* win)
        {
            setWindowSize(
                    win, {
                    ctx()->windowSize.x,
                    ctx()->windowSize.y + static_cast<int>(ctx()->cursorDelta.y)});
        });
	}

    void updateWindowSize()
   {
        int w, h;
        glfwGetWindowSize(ctx()->window, &w, &h);
        ctx()->windowSize = { w, h };
    }

    void updateWindowPos()
    {
        int w, h;
        glfwGetWindowPos(ctx()->window, &w, &h);
        ctx()->windowPos = {w, h};
    }

    void updateFps()
    {
        ctx()->fps = 1.0f / ctx()->deltaTime;
    }

    void updateCursorPosAndDelta()
    {
        double mx, my; // mouse position
        int wx, wy;    // Window position
        glfwGetCursorPos(ctx()->window, &mx, &my);
        glfwGetWindowPos(ctx()->window, &wx, &wy);

        ctx()->cursorDelta.x = mx + wx - ctx()->cursorPos.x;
        ctx()->cursorDelta.y = my + wy - ctx()->cursorPos.y;

        // Relative to screen
        ctx()->cursorPos = { mx + wx, my + wy};

        // Relative to window position
        ctx()->cursorRelativePos = { mx, my };
    }

    void createDefaultEditorWidgets(WidgetStore& widgetStore)
    {
        insertWidgetArray<ViewportContext>(widgetStore, "Viewport context");
        insertWidgetArray<LogViewerContext>(widgetStore, "Log viewer");
        insertWidgetArray<RTControlsContext>(widgetStore, "RT Controls");
#ifndef NDEBUG
        insertWidgetArray<WidgetInspectorContext>(widgetStore, "Widget inpsector");
        insertWidgetArray<ImGuiStackToolWidgetContext>(widgetStore, "stack tool");
        insertWidgetArray<ImGuiMetricsWidgetContext>(widgetStore, "metrics tool");
#endif
    }

    std::vector<FilePath> filesInsideDirectory()
    {
        return std::vector<FilePath>();
    }

    void* getWindowHandle()
    {
        return ctx()->window;
    }

    void logVendorInfo()
    {
        RT_LOG_MSG("vendor: {}", getGPUVendor());
        RT_LOG_MSG("renderer: {}", getRenderer());
        RT_LOG_MSG("GL version: {}", getGLVersion());
        RT_LOG_MSG("GLSL version: {}", getGLSLVersion());
    }

    float getDeltaTime()
    {
        return ctx()->deltaTime;
    }

    float getFps()
    {
        return ctx()->fps;
    }

    GLFWwindow* getCurrentWindowHandle()
    {
        return ctx()->window;
    }

    Vec2i getWindowSize()
    {
        return ctx()->windowSize;
    }

    void maximizeWindow()
    {
        if (!ctx()->windowMaximized)
        {
            RT_LOG_MSG("Window maximized");
            glfwMaximizeWindow(ctx()->window);
        }
        else
        {
            RT_LOG_MSG("Window restored");
            glfwRestoreWindow(ctx()->window);
        }
        ctx()->windowMaximized ^= 1;
    }

    void minimizeWindow()
    {
        RT_LOG_MSG("Window minimzed");
        glfwIconifyWindow(ctx()->window);
    }
}