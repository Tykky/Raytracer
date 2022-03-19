#include "Editor.h"
#include "logging/Logging.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <stdexcept>
#include <string>
#include "editor/Styles/DarkTheme.h"
#include <memory>
#include "ImFileDialog.h"
#include "core/Raytracer.h"

namespace Editor
{
    struct EditorContex
    {
        GLFWwindow*  editorHandle = nullptr;
        float        deltaTime    = 0.0f;
        float        mouseScroll  = 0.0f;
        WidgetStore  widgetStore;
        TextureStore textureStore;
        Raytracer    raytracer;
    };

    static EditorContex ctx;

    void init(GLFWwindow* window, const Options &options)
    {
        ctx.editorHandle = window;
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (options.enableViewports)
        {
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        }
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        execDarkTheme(); // use dark theme by default

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 440");

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
        glfwSetScrollCallback(ctx.editorHandle, mouseScrollCallback);
    }

    GLFWwindow* createWindow(const char* title, int width, int height, const Options& options)
    {
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

            if (glewInit() != GLEW_OK)
            {
                RT_LOG_ERROR("Failed to initialize GLEW");
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
        return window;
    }

    void destroyWindow(GLFWwindow* window)
    {
        glfwDestroyWindow(window);
        RT_LOG_MSG("GLFW window destroyed");
        glfwTerminate();
        RT_LOG_MSG("GLFW terminated");
    }

    void renderLoop(GLFWwindow* window)
    {
        ImGuiIO &io = ImGui::GetIO();
        double beginTime;
        double endTime = glfwGetTime();
        while (!glfwWindowShouldClose(window))
        {
            beginTime = glfwGetTime();
            ctx.deltaTime = static_cast<float>(beginTime - endTime);

            glfwPollEvents();
            clear();
            cleanInactiveWidgets(ctx.widgetStore);
            renderGui(io);
            glfwSwapBuffers(window);
            endTime = glfwGetTime();
        }
    }

    void renderImguiDrawData()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

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
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
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
        renderImguiDrawData();
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

    int getKey(int key)
    {
        return glfwGetKey(ctx.editorHandle, key);
    }

    int getMouseButton(int button)
    {
        return glfwGetMouseButton(ctx.editorHandle, button);
    }

    int getMouseButton()
    {
        return 0;
    }

    float getMouseScroll()
    {
        return ctx.mouseScroll;
    }

    void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (ctx.mouseScroll + yoffset > 0.0f)
        {
            ctx.mouseScroll += yoffset;
        }
        else
        {
            ctx.mouseScroll = 0.1f;
        }
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

    GLFWwindow* getEditorWindowHandle()
    {
        return ctx.editorHandle;
    }
}