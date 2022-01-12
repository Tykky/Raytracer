#include "Editor.h"
#include "logging/logging.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>

namespace editor
{
   void initGui(GLFWwindow* window)
    {
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
    }

    GLFWwindow* createWindow()
    {
        glfwSetErrorCallback(windowErrorCallback);
        return nullptr;
    }

    void loadGl()
    {
        if (!glewInit())
        {
            const char* msg = "Failed to init glew";
            logError(msg);
            throw std::runtime_error(msg);
        }
    }

    void windowErrorCallback(int code, const char* description)
    {
    }

    void renderLoop(GLFWwindow* window)
    {
    }
}