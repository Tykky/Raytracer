#include <GL/glew.h>
#include "Widgets.h"
#include "imgui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <iostream>
#include <vector>
#include <functional>
#include "logging/Logging.h"
#include "ImFileDialog.h"
#include "editor/styles/DarkTheme.h"
#include "editor/Input.h"

namespace Editor
{
    // Forward declaration of functions in Editor.cpp

    float getFps();
    float getDeltaTime();
    Vec2i getWindowSize();
    void maximizeWindow();
    void minimizeWindow();

    // Widget helper implementation

    void deleteAllMarkedWidgets(WidgetStore& wStore)
    {
        std::apply(
            [](auto&... wArrays)
            {
                (deleteMarkedWidgetsFromArray(wArrays), ...);
            },
            wStore
        );
    }

    // Forward declaration of internal stuff

    void processInput(ViewportContext& ctx);

    // Internal

    template<typename Context>
    struct ImGuiScopedBegin
    {
        explicit ImGuiScopedBegin(WidgetStore* wStore, Context& ctx, ImGuiWindowFlags flags = 0)
        {
            bool open = true;
            ImGui::Begin(ctx.windowId.data(), &open, flags);
            if (!open) // when close window button is pressed
            {
                ctx.markForDelete = true;
            }
         }

        ~ImGuiScopedBegin()
        {
            ImGui::End();
        }
    };

    struct ImGuiScopedBeginChild
    {
        explicit ImGuiScopedBeginChild(const char* id)
        {
            ImGui::BeginChild(id);
        }

        ~ImGuiScopedBeginChild()
        {
            ImGui::EndChild();
        }
    };

    // drawWidget implementation

    void drawWidget(WidgetStore* wStore, TextureViewerContext& ctx)
    {
        auto begin = ImGuiScopedBegin(wStore, ctx);
        void* currentTexId = nullptr;
        const char* preview = nullptr;

        if (ctx.currentTexture)
        {
            currentTexId = reinterpret_cast<void*>(ctx.currentTexture->getTextureId());
            preview = ctx.currentTexture->getName().data();
        }

        drawTextureView(currentTexId, ctx.offset, ctx.scale);
        drawTexturePickerComboBox(preview, ctx.textureStore, ctx.currentTexture);
    }

    void drawWidget(WidgetStore* wStore, LogViewerContext& ctx)
    {
		ImGui::SetNextWindowSize({800,300});
        auto begin = ImGuiScopedBegin(wStore, ctx);
		ImGui::Checkbox("Scroll to bottom", &ctx.srollToBottom);
        auto child = ImGuiScopedBeginChild("Logtext###ID");

		if (ctx.srollToBottom)
			ImGui::SetScrollY(ImGui::GetScrollMaxY());

		for (auto& msg : logMessages())
		{
			ImGui::Text(msg.data());
		}
    }

    void drawWidget(WidgetStore* wStore, ViewportContext& ctx)
    {
		// TODO: consider moving the rendering of the viewport over to camera.
		// Right now we don't have much stuff in here but at some point the "renderer" will become more complex
		processInput(ctx);
        auto begin = ImGuiScopedBegin(wStore, ctx);
        ctx.camera.update();

		setUniform(ctx.shaderProgram.getProgramId(), "view", ctx.camera.getViewMatrix());
		setUniform(ctx.shaderProgram.getProgramId(), "projection", ctx.camera.getProjectionMatrix());

		drawToTexture(ctx.vertexbuffer, ctx.shaderProgram, ctx.framebuffer);
		drawTextureView(ctx.viewportTexture, ctx.offset, ctx.scale);
    }

    void drawWidget(WidgetStore* wStore, SettingsWidgetContext& ctx) 
    {
        auto begin = ImGuiScopedBegin(wStore, ctx);
    }

    void drawWidget(WidgetStore* wStore, WidgetInspectorContext& ctx)
    {
        auto begin = ImGuiScopedBegin(wStore, ctx);
    }

    void drawWidget(WidgetStore* wStore, ImGuiMetricsWidgetContext& ctx)
    {
        ImGui::ShowMetricsWindow();
    }

    void drawWidget(WidgetStore* wStore, ImGuiStackToolWidgetContext& ctx)
    {
        ImGui::ShowStackToolWindow();
    }

    void drawWidget(WidgetStore* wStore, RTControlsContext& ctx)
    {
        auto begin = ImGuiScopedBegin(wStore, ctx);
		ImGui::InputInt("Samples", &ctx.samples);

		if (ImGui::Button("Render"))
		{
            /*
			//m_raytracer->render(m_samples);
			if (Viewport* viewport = findPrimaryViewport(*ctx.widgetStore))
			{
				//m_textureStore->push_back({ "Render" , m_raytracer->getFramebuffer().data(), m_raytracer->getWidth(), m_raytracer->getHeight() });
				ctx.viewportTexture = ctx.textureStore->back().getTextureId();
				viewport->setViewportTexture(ctx.viewportTexture);
			}
            */
		}

		ImGui::SameLine(62.0f);

		if (ImGui::Button("Stop"))
		{
			//m_raytracer->clearFramebuffer();
		}
    }

    void drawWidget(WidgetStore* wStore, SystemInfoContext& ctx)
    {
		ImGui::SetNextWindowSize({500, 300});
        auto begin = ImGuiScopedBegin(wStore, ctx);
		ImGui::Text("GPU vendor: %s", ctx.GPUVendor.data());
		ImGui::Text("Renderer: %s", ctx.renderer.data());
		ImGui::Text("GL version: %s", ctx.GLVersion.data());
		ImGui::Text("GLSL version: %s", ctx.GLSLVersion.data());
		ImGui::Text("Framerate: %f fps", ImGui::GetIO().Framerate);
		ImGui::Text("Delta time: %f ms", getDeltaTime() * 1000);
    }

    void drawAllWidgets(WidgetStore* wStore)
    {
        std::apply
        (
            [wStore](auto&... wArrays)
            {
                (drawWidgetArray(wStore, wArrays), ...);
            },
            *wStore
		);
    }

    void drawMainMenuBar(WidgetStore& widgetStore, TextureStore& textureStore, bool drawMainMenuBorders)
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
                    insertWidgetArray<SettingsWidgetContext>(widgetStore, "settings");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("New widget"))
            {
                if (ImGui::MenuItem("Texture viewer"))
                    insertWidgetArray<TextureViewerContext>(widgetStore, "Texture viewer");
                if (ImGui::MenuItem("Log viewer"))
                    insertWidgetArray<LogViewerContext>(widgetStore, "Log viewer");
                if (ImGui::MenuItem("Viewport"))
                    insertWidgetArray<ViewportContext>(widgetStore, "viewport");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("System info"))
                {
                    insertWidgetArray<SystemInfoContext>(widgetStore, "System info");
                }
                ImGui::EndMenu();
            }

#ifndef NDEBUG
            if (ImGui::BeginMenu("Debug"))
            {
                if (ImGui::MenuItem("Widget Inspector"))
                    insertWidgetArray<WidgetInspectorContext>(widgetStore, "Widget inspector");

                if (ImGui::MenuItem("ImGui Metrics"))
                    insertWidgetArray<ImGuiMetricsWidgetContext>(widgetStore, "Imgui Metrics");

                ImGui::EndMenu();
            }
#endif
            // Use custom close, minimize and maximize buttons when OS
            // provided title bar is not used
            if (!drawMainMenuBorders)
            {
                // To align buttons to the right
                auto ws = getWindowSize();
                constexpr int width = 80.0f;
                ImGui::SameLine(ws.x - width);

                if (ImGui::Button("-"))
                    minimizeWindow();

                if (ImGui::Button("O"))
                    maximizeWindow();

                if (ImGui::Button("X"))
                    exit(0);
            }

            ImGui::EndMainMenuBar();
        }
    }

    void drawDockspace(const char* name)
    {
        auto dockspaceID = ImGui::GetID("MainDockspace###ID");
        auto io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto viewport = ImGui::GetMainViewport();

            // We need to leave some room at top to fit the main menu bar
            static const int mainMenuGap = 19;

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

    void drawTextureView(void* texId, Vec2f& offset, Vec2f& scale)
    {
        constexpr int flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

		ImVec2 size = ImGui::GetWindowSize();

		constexpr const int comboBoxGap = 40;

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

    void ImGuiImplInitGLFW(void* window)
    {
        ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(window), true);
    }

    void ImGuiImplInitGL3(const char* glslVer)
    {
        ImGui_ImplOpenGL3_Init(glslVer);
    }

    void ImGuiImplGLNewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void ImGuiImplGLFWNewFrame()
    {
        ImGui_ImplGlfw_NewFrame();
    }

    void ImGuiRenderDrawData()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiRender()
    {
        ImGui::Render();
    }

    // Internal implementation
    
    void processInput(ViewportContext& ctx)
    {
        float scroll = getMouseScroll();
        if (getKey(KeyCode::KEY_LEFT_ALT) == StatusCode::PRESS && ctx.camera.distance != scroll)
        {
            ctx.camera.distance = scroll * 0.5f;
            ctx.camera.update();
        }

		auto cPos = getCursorPos();

		// This simply prevents "snapping" the camera when we process mouse input for the first time.
		// We don't want to be using the "old" values for prevMouseX and prevMouseDeltaY.
        if (getKey(KeyCode::KEY_LEFT_ALT) == StatusCode::RELEASE && getMouseButton(MouseCode::MOUSE_BUTTON_1) == StatusCode::RELEASE)
		{
			ctx.prevMousePos.x = cPos.x;
			ctx.prevMousePos.y = cPos.y;
		}

		float mousePoxXDelta = static_cast<float>(cPos.x) - ctx.prevMousePos.x;
		float mousePosYDelta = static_cast<float>(cPos.y) - ctx.prevMousePos.y;

		ctx.prevMousePos.x = cPos.x;
		ctx.prevMousePos.y = cPos.y;

        // Move sideways
        if (getKey(KeyCode::KEY_LEFT_ALT) == StatusCode::PRESS && getMouseButton(MouseCode::MOUSE_BUTTON_3) == StatusCode::PRESS)
        {
            glm::vec3 x = ctx.camera.getRight() * mousePoxXDelta * 0.01f;
            glm::vec3 y = ctx.camera.getUp() * mousePosYDelta * 0.01f;
            ctx.camera.offset += x;
            ctx.camera.offset -= y;
            ctx.camera.target += x;
            ctx.camera.target -= y;
            ctx.camera.update();
		}

        // Rotation
        if (getKey(KeyCode::KEY_LEFT_ALT) == StatusCode::PRESS && getMouseButton(MouseCode::MOUSE_BUTTON_1) == StatusCode::PRESS)
        {
            float newYaw = ctx.camera.yaw += mousePoxXDelta * ctx.camera.speed;
            float newPitch = ctx.camera.pitch -= mousePosYDelta * ctx.camera.speed;

            if (newYaw < 0.1f && newYaw >= 0.0f)
                newYaw = 0.1f;

            if (newYaw > -0.1f && newYaw <= 0.0f)
                newYaw = -0.1f;

            if (newPitch > 89.9f)
                newPitch = 89.9f;

            if (newPitch < -89.9f)
                newPitch = -89.9f;

            ctx.camera.yaw = newYaw;
            ctx.camera.pitch = newPitch;
            ctx.camera.update();
        }

        // Move forward and backwards
        if (getKey(KeyCode::KEY_LEFT_ALT) == StatusCode::PRESS && getMouseButton(MouseCode::MOUSE_BUTTON_2) == StatusCode::PRESS)
        {
            glm::vec3 x = ctx.camera.getDir() * mousePosYDelta * 0.1f;
            ctx.camera.offset += x;
            ctx.camera.target += x;
            ctx.camera.update();
        }
    }
}