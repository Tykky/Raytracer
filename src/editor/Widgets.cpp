#include "Widgets.h"
#include "imgui.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Graphics.h"
#include <vector>
#include "logging/Logging.h"

namespace Editor
{
    Widget::Widget(const char* name, unsigned int id) :
        m_name(name), m_id(id) {}

    TextureViewer::TextureViewer(const char* name, unsigned int id, TextureStore* textureStore) :
            Widget(name, id), m_TEXTURE_STORE(textureStore) {}

    void TextureViewer::draw()
    {
        static Gfx::GLtexture* currentItem = nullptr;

        static ImVec2 offset{0.0f, 0.0f};
        static ImVec2 scale{1.0f, 1.0f};

        zoomTextureWhenScrolled(scale.x, scale.y);

        int  flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        if (m_open && ImGui::Begin(m_name.data(), &m_open))
        {
            const char* preview = currentItem ? currentItem->name.data() : "";
            void* texId = currentItem ? (void*)currentItem->textureID : nullptr;

            ImGui::SetNextWindowSize(ImVec2(300, 200));
            if (ImGui::BeginChild("Texture viewer"))
            {
                auto size = ImGui::GetWindowSize();
                size = ImVec2(size.x * scale.x, size.y * scale.y);
                ImGui::Image(texId, ImVec2(300, 200));
                ImGui::EndChild();
            }

            if (ImGui::BeginCombo("Textures", preview))
            {
                for (int i = 0; i < m_TEXTURE_STORE->size(); ++i)
                {
                    auto* tex = &m_TEXTURE_STORE->at(i);
                    bool isSelected = (tex && currentItem && currentItem->textureID == tex->textureID);
                    if (ImGui::Selectable(tex->name.data(), isSelected))
                        currentItem = tex;
                }
                ImGui::EndCombo();
            }
            ImGui::End();
        }
    }

    LogViewer::LogViewer(unsigned int id) :
            Widget("Logs", id) {}

    void LogViewer::draw()
    {
        ImGui::SetNextWindowSize(ImVec2(300, 200));
        if (m_open && ImGui::Begin(m_name.data(), &m_open))
        {
            ImGui::BeginChild("Logtext###ID");
            auto logs = getLogMessages();

            for (auto& message : (*logs))
            {
                pushMessagetypeImGuiStyleVar(message.type);
                ImGui::Text(message.message.data());
                ImGui::PopStyleVar(1);
            }
            ImGui::EndChild();
            ImGui::End();
        }
    }

    void drawMainMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Widgets"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io)
    {
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto viewport = ImGui::GetMainViewport();

            // We need to leave some room at top to fit the main menu bar
            static const int mainMenuGap = 18;

            ImVec2 pos = viewport->Pos;
            pos = {pos.x, pos.y + mainMenuGap};
            ImGui::SetNextWindowPos(pos);

            ImVec2 size = viewport->Size;
            size = { size.x, size.y - mainMenuGap };
            ImGui::SetNextWindowSize(ImVec2(size));

            ImGui::SetNextWindowViewport(viewport->ID);

            // We don't need rounding for the main dockspace
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

            // We don't need any of this either for the main dockspace
            auto windowFlags = ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize   |
                               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse      | ImGuiWindowFlags_NoNavFocus |
                               ImGuiWindowFlags_NoBringToFrontOnFocus;

            if (ImGui::Begin(name, nullptr, windowFlags))
            {
                ImGui::PopStyleVar(3);
                const auto dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
                // Make this window as a dockspace
                ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
                ImGui::End();
            }
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

    void pushMessagetypeImGuiStyleVar(MessageType type)
    {
        switch (type)
        {
            case MessageType::ERROR:
                ImGui::PushStyleVar(ImGuiCol_Text, IM_COL32(255, 0, 0 ,255));
                break;
            case MessageType::WARNING:
                ImGui::PushStyleVar(ImGuiCol_Text, IM_COL32(255, 128, 0 ,255));
                break;
            default:
                ImGui::PushStyleVar(ImGuiCol_Text, IM_COL32(255, 255, 255 ,255));
                break;
        }
    }
}