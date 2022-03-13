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
    static GLFWwindow* EDITORWINDOWHANDLE = nullptr;
    static float DELTATIME = 0.0f;
    static float MOUSESCROLL = 0.0f;

    static WidgetStore  WIDGETSTORE;
    static TextureStore TEXTURESTORE;

    static int RTWIDTH = 800;
    static int RTHEIGHT = 600;
    static RTCamera      RTCAMERA;
    static RTPrimitives  RTPRIMITIVES;
    static RTAccelStruct RTACCELSTRUCT;
    static Raytracer     RAYTRACER(nullptr, &RTCAMERA, RTWIDTH, RTHEIGHT);


    void init(GLFWwindow* window, const Options &options)
    {
        EDITORWINDOWHANDLE = window;
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

        createDefaultEditorWidgets();
        glfwSetScrollCallback(EDITORWINDOWHANDLE, mouseScrollCallback);
    }

    void cleanup()
    {
    }

    GLFWwindow* createWindow(const char* title, int width, int height, const Options& options)
    {
        glfwSetErrorCallback(windowErrorCallback);
        GLFWwindow* window;
        if (glfwInit())
        {
            logMsg("GLFW initialized");
            if (!options.enableMainWindowBorders)
            {
                glfwWindowHint(GLFW_DECORATED, false);
            }

            window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwMakeContextCurrent(window);
            logMsg("GLFW window created");

            if (!options.enableVsync)
            {
                logMsg("Vsync disabled");
                glfwSwapInterval(0);
            }
            else
            {
                logMsg("Vsync enabled");
            }

            if (glewInit() != GLEW_OK)
            {
                logError("Failed to initialize GLEW");
                abort();
            }
            else
            {
                logMsg("GLEW initialized");
            }
        }
        else
        {
            logError("Failed to initialize GLEW");
            abort();
        }
        return window;
    }

    void destroyWindow(GLFWwindow* window)
    {
        glfwDestroyWindow(window);
        logMsg("GLFW window destroyed");
        glfwTerminate();
        logMsg("GLFW terminated");
    }

    void renderLoop(GLFWwindow* window)
    {
        ImGuiIO &io = ImGui::GetIO();
        double beginTime;
        double endTime = glfwGetTime();
        while (!glfwWindowShouldClose(window))
        {
            beginTime = glfwGetTime();
            DELTATIME = static_cast<float>(beginTime - endTime);

            glfwPollEvents();
            clear();
            cleanInactiveWidgets(WIDGETSTORE);
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
        logError("[GLFW CALLBACK] (" + std::to_string(code) + ") " + description);
    }

    void drawEditor(const ImGuiIO& io)
    {
        drawMainMenuBar(WIDGETSTORE, TEXTURESTORE);
        drawImFileDialogAndProcessInput();
        auto dockspaceID = ImGui::GetID("MainDockspace###ID");
        drawDockspace("Main", dockspaceID, io);

        for (auto& widget : WIDGETSTORE)
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

    void createDefaultEditorWidgets()
    {
        WIDGETSTORE.push(std::make_unique<Viewport>());
        WIDGETSTORE.push(std::make_unique<LogViewer>());
        WIDGETSTORE.push(std::make_unique<RTControls>(&RAYTRACER, &WIDGETSTORE, &TEXTURESTORE));
#ifndef NDEBUG
        WIDGETSTORE.push(std::make_unique<WidgetInspector>(&WIDGETSTORE));
        WIDGETSTORE.push(std::make_unique<ImGuiMtericsWidget>());
        WIDGETSTORE.push(std::make_unique<ImguiStackToolWidget>());
#endif
    }

    std::vector<FilePath> filesInsideDirectory()
    {
        return std::vector<FilePath>();
    }

    int getKey(int key)
    {
        return glfwGetKey(EDITORWINDOWHANDLE, key);
    }

    int getMouseButton(int button)
    {
        return glfwGetMouseButton(EDITORWINDOWHANDLE, button);
    }

    int getMouseButton()
    {
        return 0;
    }

    float getMouseScroll()
    {
        return MOUSESCROLL;
    }

    void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (MOUSESCROLL + yoffset > 0.0f)
        {
            MOUSESCROLL += yoffset;
        }
        else
        {
            MOUSESCROLL = 0.1f;
        }
    }

    float getDeltaTime()
    {
        return DELTATIME;
    }

    GLFWwindow* getEditorWindowHandle()
    {
        return EDITORWINDOWHANDLE;
    }
}