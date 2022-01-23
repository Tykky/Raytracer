#include "Widgets.h"
#include "imgui.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Graphics.h"
#include <vector>
#include "logging/Logging.h"

namespace Editor
{
    Widget::Widget(const char* name) :
        m_name(name) {}

    TextureViewer::TextureViewer(TextureStore* textureStore) :
        Widget("Texture viewer"), m_textStore(textureStore) {}

    void TextureViewer::draw()
    {
        static Gfx::GLtexture* currentItem = nullptr;

        static ImVec2 offset{0.0f, 0.0f};
        static ImVec2 scale{800.0f, 600.0f};

        int  flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        if (m_open)
        {
            ImGui::Begin(m_name.data(), &m_open, flags);
            const char* preview = currentItem ? currentItem->name.data() : "";
            void* texId = currentItem ? (void*)currentItem->textureID : nullptr;

            zoomTextureWhenScrolled(scale.x, scale.y);
            moveTextureWhenDragged(offset.x, offset.y);

            ImVec2 size = ImGui::GetWindowSize();

            ImGui::BeginChild("Texture viewer", ImVec2(size.x, size.y - 70), false, flags);
            ImGui::SetCursorPos(offset);
            ImGui::Image(texId, ImVec2(scale.x, scale.y));
            ImGui::EndChild();


            if (ImGui::BeginCombo("Textures", preview))
            {
                for (int i = 0; i < m_textStore->size(); ++i)
                {
                    auto *tex = &m_textStore->at(i);
                    bool isSelected = (tex && currentItem && currentItem->textureID == tex->textureID);
                    if (ImGui::Selectable(tex->name.data(), isSelected))
                        currentItem = tex;
                }
                ImGui::EndCombo();
            }
            ImGui::End();
        }
    }

    LogViewer::LogViewer() :
            Widget("Logs") {}

    void LogViewer::draw()
    {
        static bool scrollToBottom = true;

        if (m_open)
        {
            ImGui::Begin(m_name.data(), &m_open);
            ImGui::Checkbox("Scroll to bottom", &scrollToBottom);

            ImGui::SameLine(170.0f);
            if (ImGui::Button("clear"))
                clearLogs();

            ImGui::BeginChild("Logtext###ID");

            if (scrollToBottom)
                ImGui::SetScrollY(ImGui::GetScrollMaxY());

            int size;
            const Message* logMessages = getLogMessages(size);

            for (int i = 0; i < size; i++)
            {
                pushMessagetypeImGuiStyleVar(logMessages[i].type);
                ImGui::Text(logMessages[i].message.data());
                ImGui::PopStyleVar(1);
            }

            ImGui::EndChild();
            ImGui::End();
        }
    }

    WidgetInspector::WidgetInspector(WidgetStore* widgetStore) :
            Widget("Widget Inspector"), m_widgetStore(widgetStore) {}

    void WidgetInspector::draw()
    {
        if (m_open)
        {
            static int current;
            ImGui::Begin(m_name.data(), &m_open);
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
            ImGui::ResetMouseDragDelta();
            offsetX += mouseDelta.x;
            offsetY += mouseDelta.y;
        }
    }

    void zoomTextureWhenScrolled(float& width, float& height)
    {
        float zoom = 1 + ImGui::GetIO().MouseWheel / 10;
        if (zoom > 0.0f && width * zoom > 0 && height * zoom > 0)
        {
            width *= zoom;
            height *= zoom;
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