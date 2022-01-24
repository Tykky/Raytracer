#include "Editor.h"
#include "logging/Logging.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <stdexcept>
#include <string>
#include "Style.h"
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
        execImGuiStyle();

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

        createDefaultEditorWidgets(widgetStore, textureStore);
    }

    GLFWwindow* createWindow(const char* title, int width, int height, const Options& options)
    {
        glfwSetErrorCallback(windowErrorCallback);
        GLFWwindow* window;
        if (glfwInit())
        {
            if (!options.enableMainWindowBorders)
            {
                glfwWindowHint(GLFW_DECORATED, false);
            }

            window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwMakeContextCurrent(window);

            if (!options.enableVsync)
                glfwSwapInterval(0);

            if (glewInit() != GLEW_OK)
            {
                logError("Failed to initialize GLEW");
                abort();
            }
        }
        else
        {
            logMsg("Failed to initialize GLEW");
        }
        return window;
    }

    void destroyWindow(GLFWwindow* window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void renderLoop(GLFWwindow* window, WidgetStore& widgetStore, TextureStore& textureStore)
    {
        ImGuiIO &io = ImGui::GetIO();
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
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
        logError("[GLFW] (" + std::to_string(code) + ") " + description);
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

    void cleanInactiveWidgets(WidgetStore& widgetStore)
    {
        for (int i = 0; i < widgetStore.size(); ++i)
        {
            if (!widgetStore[i]->isOpen())
                widgetStore.erase(i);
        }
    }
}