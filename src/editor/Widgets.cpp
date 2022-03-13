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
        void* currentTexId = m_currentTexture ? (void*)m_currentTexture->getTextureId() : nullptr;
        const char* preview = m_currentTexture ? m_currentTexture->getName().data() : "";
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
            ImGui::SetNextWindowSize({800,300});
            ImGui::Begin(m_windowId.data(), &m_open);
            ImGui::Checkbox("Scroll to bottom", &m_srollToBottom);

            ImGui::BeginChild("Logtext###ID");

            if (m_srollToBottom)
                ImGui::SetScrollY(ImGui::GetScrollMaxY());

            for (auto& msg : logMessages())
            {
                ImGui::Text(msg.data());
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

    std::vector<std::unique_ptr<Widget>>::const_iterator WidgetStore::begin() const
    {
        return m_widgets.begin();
    }

    std::vector<std::unique_ptr<Widget>>::const_iterator WidgetStore::end() const
    {
        return m_widgets.end();
    }

#ifndef NDEBUG
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
#endif

    SettingsWidget::SettingsWidget() :
            Widget("Settings") {}

    void SettingsWidget::draw()
    {
        static const char* preview = "Dark";
        if (m_open)
        {
            ImGui::SetNextWindowSize({300, 200});
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
            Widget("Viewport"), m_vertexbuffer(static_cast<const float*>(defaultCubeData), sizeof(defaultCubeData))
    {
        // Create default color attachment with depth buffer (color attachment 0)
        m_framebuffer.addColorAttachment({m_resX, m_resY, true});
        setViewportTextureToColorAttachment(0);
        m_shaderProgram.addShader("data/shaders/vert.glsl", ShaderType::VERTEX);
        m_shaderProgram.addShader("data/shaders/frag.glsl", ShaderType::FRAGMENT);
        m_shaderProgram.link();
    }

    void Viewport::draw()
    {
        if (m_open)
        {
            // TODO: consider moving the rendering of the viewport over to camera.
            // Right now we don't have much stuff in here but at some point the "renderer" will become more complex
            processInput();
            ImGui::Begin(m_windowId.data());

            setUniform(m_shaderProgram.getProgramId(), "view", m_camera.getViewMatrix());
            setUniform(m_shaderProgram.getProgramId(), "projection", m_camera.getProjectionMatrix());

            drawToTexture(m_vertexbuffer, m_shaderProgram, m_framebuffer);
            drawTextureView(m_viewportTexture, m_offset, m_scale, m_open);

            if (ImGui::Button("Toggle wireframe"))
            {
                m_wireframe ^= 1;
                if (m_wireframe)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            ImGui::End();
        }
    }

    void Viewport::setViewportTexture(const RenderTexture& renderTexture)
    {
        m_viewportTexture = reinterpret_cast<void*>(renderTexture.id());
    }

    void Viewport::setViewportTexture(unsigned int texID)
    {
        m_viewportTexture = reinterpret_cast<void*>(texID);
    }

    void Viewport::setViewportTextureToColorAttachment(unsigned int attachment)
    {
        assert(attachment < m_framebuffer.getNumColorAttachments());
        m_viewportTexture = reinterpret_cast<void*>(m_framebuffer.getColorAttachments()[attachment].id());
    }

    void Viewport::processInput()
    {
        static float prevMouseX = 0.0f;
        static float prevMouseY = 0.0f;

        float scroll = getMouseScroll();
        if (m_camera.distance != scroll)
        {
            m_camera.distance = scroll * 0.5f;
            m_camera.update();
        }

		double mousePosX, mousePosY;
		glfwGetCursorPos(getEditorWindowHandle(), &mousePosX, &mousePosY);

		// This simply prevents "snapping" the camera when we process mouse input for first time.
		// We don't want to be using the "old" values for prevMouseX an prevMouseDeltaY.
        if (getKey(GLFW_KEY_LEFT_ALT) == GLFW_RELEASE && getMouseButton(GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
		{
			prevMouseX = mousePosX;
			prevMouseY = mousePosY;
		}

		float mousePoxXDelta = static_cast<float>(mousePosX) - prevMouseX;
		float mousePosYDelta = static_cast<float>(mousePosY) - prevMouseY;

		prevMouseX = mousePosX;
		prevMouseY = mousePosY;

        // Movement
        if (getKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS && getMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            glm::vec3 x = m_camera.getRight() * mousePoxXDelta * 0.01f;
            glm::vec3 y = m_camera.getUp() * mousePosYDelta * 0.01f;
            m_camera.offset += x;
            m_camera.offset -= y;
            m_camera.target += x;
            m_camera.target -= y;
            m_camera.update();
		}

        // Rotation
        if (getKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS && getMouseButton(GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {
            float newYaw = m_camera.yaw += mousePoxXDelta * m_camera.speed;
            float newPitch = m_camera.pitch -= mousePosYDelta * m_camera.speed;

            if (newYaw < 0.1f && newYaw >= 0.0f)
                newYaw = 0.1f;

            if (newYaw > -0.1f && newYaw <= 0.0f)
                newYaw = -0.1f;

            if (newPitch > 89.9f)
                newPitch = 89.9f;

            if (newPitch < -89.9f)
                newPitch = -89.9f;

            m_camera.yaw = newYaw;
            m_camera.pitch = newPitch;
            m_camera.update();
        }
    }

    RTControls::RTControls(Raytracer* raytracer, WidgetStore* widgetstore, TextureStore* textureStore) :
        Widget("RTControls"), m_raytracer(raytracer), m_widgetStore(widgetstore), m_textureStore(textureStore)
    {}

    void RTControls::draw()
    {
        if (m_open)
        {
            ImGui::Begin(m_windowId.data());
            ImGui::InputInt("Samples", &m_samples);

            if (ImGui::Button("Render"))
            {
                m_raytracer->render(m_samples);
                if (Viewport* viewport = findPrimaryViewport(*m_widgetStore))
                {
                    m_textureStore->push_back({ "Render" , m_raytracer->getFramebuffer().data(), m_raytracer->getWidth(), m_raytracer->getHeight() });
                    m_viewportTexture = m_textureStore->back().getTextureId();
                    viewport->setViewportTexture(m_viewportTexture);
                }
            }

            if (ImGui::Button("asd"))
            {
                if (Viewport* viewport = findPrimaryViewport(*m_widgetStore))
                {
                    m_textureStore->push_back({ "Render" , m_raytracer->getFramebuffer().data(), m_raytracer->getWidth(), m_raytracer->getHeight() });
                    m_viewportTexture = m_textureStore->back().getTextureId();
                    viewport->setViewportTexture(m_viewportTexture);
                }

            }


            ImGui::SameLine(62.0f);

            if (ImGui::Button("Stop"))
            {
                m_raytracer->clearFramebuffer();
            }

			ImGui::End();
        }
    }

    SystemInfo::SystemInfo() :
        Widget("SystemInfo"), m_GPUVendor(getGPUVendor()), m_renderer(getRenderer()),
        m_GLVersion(getGLVersion()), m_GLSLVersion(getGLSLVersion())
    {}

    void SystemInfo::draw()
    {
        if (m_open)
        {
            ImGui::SetNextWindowSize({500, 300});
            ImGui::Begin(m_windowId.data(), &m_open);
            ImGui::Text("GPU vendor: %s", m_GPUVendor.data());
            ImGui::Text("Renderer: %s", m_renderer.data());
            ImGui::Text("GL version: %s", m_GLVersion.data());
            ImGui::Text("GLSL version: %s", m_GLSLVersion.data());
            ImGui::Text("Framerate: %f fps", ImGui::GetIO().Framerate);
            ImGui::Text("Delta time: %f ms", getDeltaTime() * 1000000.0f);
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
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("System info"))
                {
                    widgetStore.push(std::make_unique<SystemInfo>());
                }
                ImGui::EndMenu();
            }

#ifndef NDEBUG
            if (ImGui::BeginMenu("Debug"))
            {
                if (ImGui::MenuItem("Widget Inspector"))
                    widgetStore.push(std::make_unique<WidgetInspector>(&widgetStore));

                if (ImGui::MenuItem("ImGui Metrics"))
                    widgetStore.push(std::make_unique<ImGuiMtericsWidget>());
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

            if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
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
                bool isSelected = (currentTexture && currentTexture->getTextureId() == tex.getTextureId());
                if (ImGui::Selectable(tex.getName().data(), isSelected))
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

    void cleanInactiveWidgets(WidgetStore& widgetStore)
    {
        for (int i = 0; i < widgetStore.size(); ++i)
        {
            if (!widgetStore[i]->isOpen())
                widgetStore.erase(i);
        }
    }

    Viewport* findPrimaryViewport(const WidgetStore& widgetStore)
    {
        for (const auto& widget : widgetStore)
        {
            if (Viewport* viewport = dynamic_cast<Viewport*>(widget.get()))
                return viewport;
        }
        logWarning("No primary viewport found!");
        return nullptr;
    }
}