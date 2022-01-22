#include "Editor.h"
#include "logging/Logging.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <stdexcept>
#include <string>
#include "Style.h"
#include <memory>

// We store all widgets in this
static WidgetStore  WIDGET_STORAGE;
// We store all textures visible to editor here
static TextureStore TEXTURE_STORE;
// We create unique ids for widgets by simply incrementing this
static unsigned int LAST_WIDGET_ID = 0;

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

        createDefaultEditorWidgets();
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

    void renderLoop(GLFWwindow* window)
    {
        ImGuiIO &io = ImGui::GetIO();
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            Gfx::clear();
            renderGui(io);
            glfwSwapBuffers(window);
        }
    }

    void freeTexture(unsigned int textureID)
    {
        for (int i = 0; i < TEXTURE_STORE.size(); ++i)
        {
            if (TEXTURE_STORE[i].textureID == textureID)
            {
                TEXTURE_STORE.erase(TEXTURE_STORE.begin() + i);
                Gfx::deleteTexture(textureID);
            }
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

    void drawEditor(const ImGuiIO& io)
    {
        drawMainMenuBar();
        auto dockspaceID = ImGui::GetID("MainDockspace###ID");
        drawDockspace("Main", dockspaceID, io);
        for (int i = 0; i < WIDGET_STORAGE.size(); ++i)
        {
            WIDGET_STORAGE[i]->draw();
        }
    }

    void renderGui(ImGuiIO& io)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawEditor(io);

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

    void createDefaultEditorWidgets()
    {
        WIDGET_STORAGE.push_back(std::make_unique<TextureViewer>("Texture viewer", createWidgetId(), &TEXTURE_STORE));
        WIDGET_STORAGE.push_back(std::make_unique<LogViewer>(createWidgetId()));
    }

    unsigned int createWidgetId() { return LAST_WIDGET_ID++; }
}