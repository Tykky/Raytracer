#include "Editor.h"
#include "logging/Logging.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>
#include <string>
#include "Style.h"

namespace Editor
{
   void init(GLFWwindow* window, const Options& options)
    {
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
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

    GLFWwindow* createWindow(int width, int height, const char* title)
    {
        glfwSetErrorCallback(windowErrorCallback);
        GLFWwindow* window = nullptr;
        if (glfwInit())
        {
            window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwMakeContextCurrent(window);
            glfwSwapInterval(0);
            // Load OpenGL extensions
            if (glewInit() != GLEW_OK)
            {
                const char* msg = "[GLEW] Failed to initialize";
                logMsg(msg);
                throw std::runtime_error(msg);
            }
        }
        else
        {
            const char* msg = "[GLFW] Failed to initialize";
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

    void windowErrorCallback(int code, const char* description)
    {
       std::string msg = "[GLFW] (" + std::to_string(code) + ") " + description;
       logError(msg.data());
    }

    void renderLoop(GLFWwindow* window)
    {
       ImGuiIO& io = ImGui::GetIO();
       while(!glfwWindowShouldClose(window))
       {
           glfwPollEvents();
           glClear(GL_COLOR_BUFFER_BIT);
           renderGui(io);
           glfwSwapBuffers(window);
       }
    }

    void renderImguiDrawData()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void renderGui(ImGuiIO& io)
    {
       ImGui_ImplOpenGL3_NewFrame();
       ImGui_ImplGlfw_NewFrame();
       ImGui::NewFrame();

       ImGui::ShowDemoWindow();

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
}