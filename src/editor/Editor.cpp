#include "Editor.h"
#include "logging/Logging.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <stdexcept>
#include <string>
#include "Style.h"
#include "Widgets.h"

namespace Editor
{
    void init(GLFWwindow *window, const Options &options)
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
    }

    GLFWwindow* createWindow(const char* title, int width, int height, const Options& options)
    {
        glfwSetErrorCallback(windowErrorCallback);
        GLFWwindow *window;
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
                const char *msg = "[GLEW] Failed to initialize";
                logMsg(msg);
                throw std::runtime_error(msg);
            }
        }
        else
        {
            const char *msg = "[GLFW] Failed to initialize";
            logMsg(msg);
            throw std::runtime_error(msg);
        }
        return window;
    }

    void destroyWindow(GLFWwindow* window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void renderLoop(GLFWwindow* window, Widget* widgets, std::size_t numWidgets)
    {
        ImGuiIO &io = ImGui::GetIO();
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT);
            renderGui(io, widgets, numWidgets);
            glfwSwapBuffers(window);
        }
    }

    void renderImguiDrawData()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void windowErrorCallback(int code, const char* description)
    {
        std::string msg = "[GLFW] (" + std::to_string(code) + ") " + description;
        logError(msg.data());
    }

    void drawEditor(const ImGuiIO& io, Widget* widgets, std::size_t numWidgets)
    {
        drawMainMenuBar();
        auto dockspaceID = ImGui::GetID("MainDockspace###ID");
        drawDockspace("Main", dockspaceID, io);
        if (widgets)
        {
            for (int i = 0; i < numWidgets; ++i)
            {
                widgets[i].draw();
            }
        }
    }

    void renderGui(ImGuiIO &io, Widget* widgets, std::size_t numWidgets)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawEditor(io, widgets, numWidgets);

        ImGui::EndFrame();
        ImGui::Render();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* window = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(window);
        }

        renderImguiDrawData();
    }

    std::vector<Widget> createDefaultEditorWidgets()
    {
        std::vector<Widget> widgets;
        return widgets;
    }
}