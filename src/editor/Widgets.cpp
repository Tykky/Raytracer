 #include <GL/glew.h>
#include "Widgets.h"
#include "imgui.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Graphics.h"
#include <vector>
#include "logging/Logging.h"
#include "ImFileDialog.h"
#include "editor/styles/DarkTheme.h"

namespace Editor
{
    Widget::Widget(const char* name) :
        m_name(name)  {}

    TextureViewer::TextureViewer(TextureStore* textureStore) :
        Widget("Texture viewer"), m_textStore(textureStore) {}

    void TextureViewer::draw()
    {
        ImGui::Begin(m_windowId.data(), &m_open);
        void* currentTexId = m_currentTexture ? (void*)m_currentTexture->textureID : nullptr;
        const char* preview = m_currentTexture ? m_currentTexture->name.data() : "";
        drawTextureView(currentTexId, m_offset, m_scale, m_open);
        drawTexturePickerComboBox(preview, m_textStore, m_currentTexture);
        ImGui::End();
    }

    LogViewer::LogViewer() :
            Widget("Logs") {}

    void LogViewer::draw()
    {
        if (m_open)
        {
            ImGui::Begin(m_windowId.data(), &m_open);
            ImGui::Checkbox("Scroll to bottom", &m_srollToBottom);

            ImGui::SameLine(170.0f);
            if (ImGui::Button("clear"))
                getLogMessages().clear();

            ImGui::BeginChild("Logtext###ID");

            if (m_srollToBottom)
                ImGui::SetScrollY(ImGui::GetScrollMaxY());

            for (auto& msg : getLogMessages())
            {
                pushMessagetypeImGuiStyleVar(msg.type);
                ImGui::Text(msg.message.data());
                ImGui::PopStyleVar(1);
            }

            ImGui::EndChild();
            ImGui::End();
        }
    }

    unsigned int WidgetStore::size() const
    {
        return m_widgets.size();
    }

    Widget* WidgetStore::operator[](int idx) const
    {
        assert(idx < m_widgets.size());
        return m_widgets[idx].get();
    }

    void WidgetStore::push(std::unique_ptr<Widget>&& widget)
    {
        widget->setId(m_currentUniqueIdx++);
        widget->setWindowId(widget->getName() + "###" + std::to_string(widget->getId()));
        m_widgets.push_back(std::move(widget));
    }

    void WidgetStore::erase(int idx)
    {
        assert(idx < m_widgets.size());
        m_widgets.erase(m_widgets.begin() + idx);
    }

    std::vector<std::unique_ptr<Widget>>::iterator WidgetStore::begin()
    {
        return m_widgets.begin();
    }

    std::vector<std::unique_ptr<Widget>>::iterator WidgetStore::end()
    {
        return m_widgets.end();
    }

    WidgetInspector::WidgetInspector(WidgetStore* widgetStore) :
            Widget("Widget Inspector"), m_widgetStore(widgetStore) {}

    void WidgetInspector::draw()
    {
        if (m_open)
        {
            ImGui::Begin(m_windowId.data(), &m_open);
            for (auto& widget : *m_widgetStore)
            {
                std::string label = widget->getName() + " id: " + std::to_string(widget->getId());
                ImGui::Text(label.data());
            }
            ImGui::End();
        }
    }

    SettingsWidget::SettingsWidget() :
            Widget("Settings") {}

    void SettingsWidget::draw()
    {
        static const char* preview = "Dark";
        if (m_open)
        {
            ImGui::Begin(m_windowId.data(), &m_open);
            if (ImGui::BeginCombo("Theme", preview))
            {
                if (ImGui::Selectable("Dark"))
                {
                    execDarkTheme();
                    preview = "Dark";
                }
                if (ImGui::Selectable("Light"))
                {
                    ImGui::StyleColorsLight();
                    preview = "Light";
                }
                if (ImGui::Selectable("classic"))
                {
                    ImGui::StyleColorsClassic();
                    preview = "Classic";
                }
                ImGui::EndCombo();
            }

            static float scale = 1.0f;
            float tmp = scale;

            if (ImGui::InputFloat("Font scale", &tmp) && tmp <= 3 && tmp >= 0.5)
            {
                scale = tmp;
                ImGui::GetIO().FontGlobalScale = scale;
            }
            ImGui::End();
        }
    }

    Viewport::Viewport() :
        Widget("Viewport") {}

    void Viewport::draw()
    {
        if (m_open)
        {
            ImGui::Begin(m_windowId.data());
            drawTextureView(nullptr, m_offset, m_scale, m_open);
            ImGui::End();
        }
    }

    void drawMainMenuBar(WidgetStore& widgetStore, TextureStore& textureStore)
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open"))
                    ifd::FileDialog::Instance().Open("Dialog", "Open a file", "");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Settings"))
                    widgetStore.push(std::make_unique<SettingsWidget>());
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("New widget"))
            {
                if (ImGui::MenuItem("Texture viewer"))
                    widgetStore.push(std::make_unique<TextureViewer>(&textureStore));
                if (ImGui::MenuItem("Log viewer"))
                    widgetStore.push(std::make_unique<LogViewer>());
                ImGui::EndMenu();
            }
#ifndef NDEBUG
            if (ImGui::BeginMenu("Debug"))
            {
                if (ImGui::MenuItem("Widget Inspector"))
                    widgetStore.push(std::make_unique<WidgetInspector>(&widgetStore));

                if (ImGui::MenuItem("ImGui Metrics"))
                    widgetStore.push(std::make_unique<ImGuiMetrics>());
                ImGui::EndMenu();
            }
#endif
            ImGui::EndMainMenuBar();
        }
    }

    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io)
    {
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto viewport = ImGui::GetMainViewport();

            // We need to leave some room at top to fit the main menu bar
            static const int mainMenuGap = 22;

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

    void drawImFileDialogAndProcessInput()
    {
        if (ifd::FileDialog::Instance().IsDone("Dialog"))
        {
            if (ifd::FileDialog::Instance().HasResult())
            {
                std::string res = ifd::FileDialog::Instance().GetResult().u8string();
            }
            ifd::FileDialog::Instance().Close();
        }
    }

    void drawTextureView(void* texId, ImVec2& offset, ImVec2& scale, bool& open)
    {
        constexpr int flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        if (open)
        {
            ImVec2 size = ImGui::GetWindowSize();

            constexpr const int comboBoxGap = 100;

            ImGui::BeginChild("TextureViewer", {size.x, size.y - comboBoxGap}, false, flags);

            if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_LeftAlt))
            {
                zoomTextureWhenScrolled(scale.x, scale.y);
                moveTextureWhenDragged(offset.x, offset.y);
            }

            // Dear Imgui draws textures upper left corner on current cursor position.
            // When we zoom the texture we want to keep it centered. We do this by
            // computing correct upper left corner so that the center of the texture is in the middle of the window.
            ImVec2 center = {(size.x - scale.x) * 0.5f + offset.x, (size.y - scale.y) * 0.5f + offset.y};

            // By moving the cursor position we can move the texture.
            ImGui::SetCursorPos(center);
            ImGui::Image(texId, ImVec2(scale.x, scale.y));
            ImGui::EndChild();
        }
    }

    void drawTexturePickerComboBox(const char* preview, TextureStore* textureStore, Texture*& currentTexture)
    {
        ImGui::BeginChild("Texture Picker", {180, 30});
        if (ImGui::BeginCombo("Textures", preview))
        {
            for (auto& tex : *textureStore)
            {
                bool isSelected = (currentTexture && currentTexture->textureID == tex.textureID);
                if (ImGui::Selectable(tex.name.data(), isSelected))
                    currentTexture = &tex;
            }
            ImGui::EndCombo();
        }
        ImGui::EndChild();
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

    void cleanInactiveWidgets(WidgetStore& widgetStore)
    {
        for (int i = 0; i < widgetStore.size(); ++i)
        {
            if (!widgetStore[i]->isOpen())
                widgetStore.erase(i);
        }
    }
}