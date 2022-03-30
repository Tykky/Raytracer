#include "Editor.h"
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include "Widgets.h"
#include "editor/Graphics.h"
#include "editor/Styles/DarkTheme.h"
#include "editor/Input.h"
#include "ImFileDialog.h"
#include "core/Raytracer.h"

namespace Editor
{
    //------------------------------//
    // Internal forward declaration //
    //------------------------------//

    void renderGui(ImGuiIO &io);
    void drawEditor(const ImGuiIO& io);
    void windowErrorCallback(int code, const char* description);
    void createDefaultEditorWidgets(WidgetStore& widgetStore);
    void logVendorInfo();
    std::vector<FilePath> filesInsideDirectory();
    GLFWwindow* getCurrentWindowHandle();

    //----------------//
    // Editor Context //
    //----------------//

    struct EditorContext 
    {
        GLFWwindow*  window       = nullptr;
        float        deltaTime    = 0.0f;
        float        mouseScroll  = 0.0f;
        Vec2D<float> cursorPos    = { 0.0f, 0.0f };
        WidgetStore  widgetStore;
        TextureStore textureStore;
        Raytracer    raytracer;
        bool         initialized  = false;
    };

    // For now there is only one context at any given time
    static EditorContext ctx;

    //---------------------------//
    // Editor API implementation //
    //---------------------------//

    void init(const Options &options)
    {
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (options.enableViewports)
        {
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        }
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        execDarkTheme(); // use dark theme by default

        ImGuiImplInitGLFW(ctx.window);
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

        createDefaultEditorWidgets(ctx.widgetStore);
        setMouseScrollCallback();
    }

    void createWindow(const char* title, int width, int height, const Options& options)
    {
        if (ctx.window)
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
                glfwWindowHint(GLFW_DECORATED, false);
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
                RT_LOG_ERROR("Failed to initialize GLEW!");
                abort();
            }
            else
            {
                RT_LOG_MSG("GLEW initialized");
                logVendorInfo();
            }
        }
        else
        {
            RT_LOG_ERROR("Failed to initialize GLEW");
            abort();
        }
		ctx.window = window;
    }

    void destroyWindow()
    {
        if(!ctx.window)
        {
            RT_LOG_WARNING("Tried to destroy window when one hasn't been created yet!");
            return;
        }

        glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(ctx.window));
        RT_LOG_MSG("GLFW window destroyed");
        glfwTerminate();
        RT_LOG_MSG("GLFW terminated");
    }

    void renderLoop()
    {
        if(!ctx.window)
        {
            RT_LOG_WARNING("Tried to enter renderloop when there is no valid window yet!");
            return;
        }

        if(!ctx.initialized)
        {
            RT_LOG_WARNING("Tried to enter renderloop when the editor is not initialized!");
        }

        GLFWwindow* glfwWindow = reinterpret_cast<GLFWwindow*>(ctx.window);
        ImGuiIO &io = ImGui::GetIO();
        double beginTime;
        double endTime = glfwGetTime();
        while (!glfwWindowShouldClose(glfwWindow))
        {
            beginTime = glfwGetTime();
            ctx.deltaTime = static_cast<float>(beginTime - endTime);

            glfwPollEvents();
            clear();
            cleanInactiveWidgets(ctx.widgetStore);
            renderGui(io);
            glfwSwapBuffers(glfwWindow);
            endTime = glfwGetTime();
        }
    }
   
    //-------------------------//
    // Internal implementation //
    //-------------------------//

    void windowErrorCallback(int code, const char* description)
    {
        RT_LOG_ERROR("[GLFW CALLBACK] ({}) {}", code, description);
    }

    void drawEditor(const ImGuiIO& io)
    {
        drawMainMenuBar(ctx.widgetStore, ctx.textureStore);
        drawImFileDialogAndProcessInput();
        auto dockspaceID = ImGui::GetID("MainDockspace###ID");
        drawDockspace("Main", dockspaceID, io);

        for (auto& widget : ctx.widgetStore)
        {
            widget->draw();
        }
    }

    void renderGui(ImGuiIO& io)
    {
        ImGuiImplGLNewFrame();
        ImGuiImplGLFWNewFrame();
        ImGui::NewFrame();

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

    void createDefaultEditorWidgets(WidgetStore& widgetStore)
    {
        widgetStore.push(std::make_unique<Viewport>());
        widgetStore.push(std::make_unique<LogViewer>());
        widgetStore.push(std::make_unique<RTControls>(&ctx.raytracer, &ctx.widgetStore, &ctx.textureStore));
#ifndef NDEBUG
        widgetStore.push(std::make_unique<WidgetInspector>(&ctx.widgetStore));
        widgetStore.push(std::make_unique<ImGuiMtericsWidget>());
        widgetStore.push(std::make_unique<ImguiStackToolWidget>());
#endif
    }

    std::vector<FilePath> filesInsideDirectory()
    {
        return std::vector<FilePath>();
    }

    GLFWwindow* getWindowHandle()
    {
        return ctx.window;
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
        return ctx.deltaTime;
    }

    GLFWwindow* getCurrentWindowHandle()
    {
        return ctx.window;
    }
}