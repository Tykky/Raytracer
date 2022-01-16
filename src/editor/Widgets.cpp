#include "Widgets.h"
#include "imgui.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Graphics.h"
#include <vector>

namespace Editor
{
    Widget::Widget(const char* name) :
        m_name(name) {}

    void Widget::close()
    {
        m_open = false;
    }

    void Widget::open()
    {
        m_open = true;
    }

    RenderView::RenderView(const char* name) :
            Widget(name) {}

    RenderView::RenderView(const char* name, GLtexture texture) :
            Widget(name), m_gltexture() {}

    void RenderView::draw()
    {
        if (m_open && ImGui::Begin(m_name, &m_open))
        {
            // Full screen texture
            const auto size = ImGui::GetWindowSize();
            ImGui::Image(nullptr, size);
            ImGui::End();
        }
    }

    void drawMainMenuBar()
    {
        struct SubMenuItem
        {
            const char* name;
            void (*menuClicked)() = nullptr;
        };

        struct MainMenuItem
        {
            const char* name;
            // TODO: Try to get rid of std::vector
            std::vector<SubMenuItem> subMenu;
        };

        static MainMenuItem topLevelMainMenu[] =
        {
            {"File", {
                {""},
                {""}
            }},
            {"View",{
                {"Renderview"}
            }},
            {"Debug",{

            }}
        };

        if (ImGui::BeginMainMenuBar())
        {
            for (auto item : topLevelMainMenu)
            {
                if (ImGui::BeginMenu(item.name))
                {
                    // Draw submenu here
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();
        }
    }

    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io)
    {
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(ImVec2(viewport->Size));
            ImGui::SetNextWindowViewport(viewport->ID);

            // We don't need rounding for the main dockspace
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

            // We don't need any of this either for main dockspace
            auto windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                               ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus;

            ImGui::Begin(name, nullptr, windowFlags);

            ImGui::PopStyleVar(3);

            const auto dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

            // Make this window as a dockspace
            ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);

            ImGui::End();
        }
    }

    void moveTextureWhenDragged(float& offsetX, float& offsetY)
    {
        if (ImGui::IsMouseDragging(0))
        {
            ImVec2 mouseDelta = ImGui::GetMouseDragDelta();
            offsetX += mouseDelta.x;
            offsetY += mouseDelta.y;
        }
    }

    void zoomTextureWhenScrolled(float& width, float& height)
    {
        float zoom = 1 + ImGui::GetIO().MouseWheel / 10;
        if (zoom > 0.0f)
        {
            width += zoom;
            height += zoom;
        }
    }
}