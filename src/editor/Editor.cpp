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

namespace Editor
{
    void init(GLFWwindow* window, const Options &options, WidgetStore& widgetStore, TextureStore& textureStore)
    {
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (options.enableViewports)
        {
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        }
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        execDarkTheme(); // use dark theme by defualt

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 440");

        // ImFIleDialog needs functions for creating and freeing textures for icons
        ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void*
        {
            return Gfx::createTexture(data, w, h, fmt);
        };

        ifd::FileDialog::Instance().DeleteTexture = [](void* tex)
        {
            Gfx::deleteTexture(tex);
        };

        // Load custom font
        io.Fonts->AddFontFromFileTTF("Roboto-Regular.ttf", 16.0f);

        createDefaultEditorWidgets(widgetStore, textureStore);
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
                logMsg("Vsync enabled");
                glfwSwapInterval(0);
            }
            else
            {
                logMsg("Vsync disabled");
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

    void renderLoop(GLFWwindow* window, WidgetStore& widgetStore, TextureStore& textureStore)
    {
        ImGuiIO &io = ImGui::GetIO();
        while (!glfwWindowShouldClose(window))
        {
            // Limit to 10 fps when no inputs received
            glfwWaitEventsTimeout(0.1);
            Gfx::clear();
            cleanInactiveWidgets(widgetStore);
            renderGui(io, widgetStore, textureStore);
            glfwSwapBuffers(window);
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

    void drawEditor(const ImGuiIO& io, WidgetStore& widgetStore, TextureStore& textureStore)
    {
        drawMainMenuBar(widgetStore, textureStore);
        drawImFileDialogAndProcessInput();
        auto dockspaceID = ImGui::GetID("MainDockspace###ID");
        drawDockspace("Main", dockspaceID, io);
        for (int i = 0; i < widgetStore.size(); ++i)
        {
            widgetStore[i]->draw();
        }
    }

    void renderGui(ImGuiIO& io, WidgetStore& widgetStore, TextureStore& textureStore)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawEditor(io, widgetStore, textureStore);

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

    void createDefaultEditorWidgets(WidgetStore& widgetStore, TextureStore& textureStore)
    {
        widgetStore.push(std::make_unique<TextureViewer>(&textureStore));
        widgetStore.push(std::make_unique<LogViewer>());
        widgetStore.push(std::make_unique<WidgetInspector>(&widgetStore));
    }
}