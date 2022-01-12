#include <GL/glew.h>
#include "Gui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <random>
#include <primitives/Sphere.h>
#include <string>
#include <chrono>
#include <algorithm>
#include "primitives/Bvhnode.h"
#include "primitives/Triangle.h"
#include "io/meshloader.h"
#include "materials/Mix.h"
#include "materials/Lambertian.h"
#include "materials/Dielectric.h"
#include "style.h"
#include "logging/logging.h"

void init(GLFWwindow *window, int &framebufferTextureId, int width, int height, char *framebuffer)
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    framebufferTextureId = setupTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
    execImGuiStyle();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().ChildRounding = 0.0f;
    ImGui::GetStyle().FrameRounding = 0.0f;
    ImGui::GetStyle().GrabRounding = 0.0f;
    ImGui::GetStyle().PopupRounding = 0.0f;
    ImGui::GetStyle().ScrollbarRounding = 0.0f;
}

void renderGui(GuiState &state)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    rightSideBarResize(state);

    displayMainMenu(state.mainmenu);
    displayRenderedImage(state);
    displayRightSideBar(state);

    if (state.displayImguiMetrics) ImGui::ShowMetricsWindow();
    if (state.displayImguiDemo) ImGui::ShowDemoWindow();
    if (state.displayImguiAbout) ImGui::ShowAboutWindow();
    if (state.displayImguiUserGUide) ImGui::ShowUserGuide();

    ImGui::Render();
}

void renderDrawData()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

namespace
{
    void displayMainMenu(const std::vector<Menuitem> &mainMenu)
    {
        if (ImGui::BeginMainMenuBar())
        {
            for (const Menuitem &item: mainMenu)
            {
              if (item.display && ImGui::BeginMenu(item.label.c_str()))
                {
                   for (const Menuitem &subitem: *item.submenu)
                    {
                        if (ImGui::MenuItem(subitem.label.c_str(), nullptr, *subitem.display))
                        {
                            *subitem.display ^= true;
                        }
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();
        }
    }

    unsigned int setupTexture()
    {
        unsigned int texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        return texture;
    }

    void displayRenderedImage(GuiState &state)
    {
        ImGui::SetNextWindowPos(ImVec2(0, state.mainMenubarHeight));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(state.windowHeight) - state.rightSideBarWidth,
                                        static_cast<float>(state.windowHeight) - state.perfMonitorHeight));
        ImGui::Begin("Rendered image", nullptr,
                     state.staticWindowFlags | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImVec2 window_size = ImGui::GetWindowSize();

        if (ImGui::IsWindowHovered())
        {
            moveTextureWhenDragged(state);
            zoomTextureWhenScrolled(state);
        }

        ImVec2 texture_center = ImVec2((window_size.x - state.textureWidth) * 0.5f + state.textureOffset.x,
                                       (window_size.y - state.textureHeight) * 0.5f + state.textureOffset.y);

        ImGui::SetCursorPos(texture_center);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, state.renderWidth, state.renderHeight,
                        GL_RGB, GL_UNSIGNED_BYTE, state.raytracer.getFramebuffer().data());

        ImGui::Image((void *) (intptr_t) state.framebufferTextureId, ImVec2(state.textureWidth, state.textureHeight));
        ImGui::End();
    }

    void displayRightSideBar(const GuiState &state)
    {
        ImGui::SetNextWindowSize(
                ImVec2(state.rightSideBarWidth, static_cast<float>(state.windowHeight) - state.mainMenubarHeight));
        ImGui::SetNextWindowPos(
                ImVec2(static_cast<float>(state.windowHeight) - state.rightSideBarWidth, state.mainMenubarHeight));
        ImGui::Begin("right_side_bar", nullptr, state.staticWindowFlags);

        if (ImGui::IsWindowHovered())
        {
            ImGui::SetWindowFocus();
        }

        //displayRenderSettingsChild(ImVec2(state.rightSideBarMaxWidth, 120), state);
        //displayCameraSettingsChild(ImVec2(state.rightSideBarMaxWidth, 120), state);
        //displayRandomizerChild(ImVec2(state.rightSideBarMaxWidth, 150), state);
        //displayObjectsChild(ImVec2(m_rightSideBarWidth, 300));
        ImGui::End();
    }

    void displayRenderSettingsChild(const ImVec2 &size, GuiState &state)
    {
        ImGui::BeginChild("render_settings", size, true);
        ImGui::Text("Render settings");
        int *res[] = {&state.renderWidth, &state.renderHeight};
        if (ImGui::InputInt2("Resolution", *res))
        {
            state.raytracer.resize(state.renderWidth, state.renderHeight);
            state.raytracer.clearFramebuffer();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state.renderWidth, state.renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         state.raytracer.getFramebuffer().data());
        }
        ImGui::InputInt("Samples", &state.renderSamples);
        if (ImGui::Button("Render") && !state.world.empty() && !state.raytracer.isRendering())
        {
            //state.bvh = std::make_shared<Bvhnode>(state.world,0, state.world.size(), 0, 1, state.randomFloat);
            //state.raytracer.setWorld(state.bvh.get());
            startRaytracer(state);
        }
        ImGui::SameLine(65);
        if (ImGui::Button("Clear"))
        {
            state.raytracer.clearFramebuffer();
        }
        ImGui::EndChild();
    }

    void displayCameraSettingsChild(const ImVec2 &size, GuiState &state)
    {
        ImGui::BeginChild("camera_Settings", size, true);
        ImGui::Text("Camera settings");
        float *pos[] = {&state.cameraPosX, &state.cameraPosY, &state.cameraPosZ};
        if (ImGui::InputFloat3("Position", *pos))
        {
            state.camera.setPos({state.cameraPosX, state.cameraPosY, state.cameraPosZ});
            state.camera.applyChanges();
        }
        float *up[] = {&state.cameraUpX, &state.cameraUpY, &state.cameraUpZ};
        if (ImGui::InputFloat3("Up pos", *up))
        {
            state.camera.setUp({state.cameraUpX, state.cameraUpY, state.cameraUpY});
            state.camera.applyChanges();
        }
        if (ImGui::SliderFloat("Fov", &state.cameraFov, 0, 110))
        {
            state.camera.setFov(state.cameraFov);
            state.camera.applyChanges();
        }
        ImGui::EndChild();
    }

    void displayRandomizerChild(const ImVec2 &size, GuiState &state)
    {
        ImGui::BeginChild("randomizer", size, true);
        ImGui::Text("Randomizer settings");
        ImGui::InputInt("Spheres", &state.randomizerSphereCOunt);
        ImGui::InputInt("Scatter", &state.randomizerScatterMultiplier);
        if (ImGui::Button("Render randomized") && !state.raytracer.isRendering())
        {
            clearObjects(state);
            randomizeWorld(state.randomizerSphereCOunt, state.randomizerScatterMultiplier, state);
            //state.bvh = std::make_shared<Bvhnode>(state.world,0, state.world.size(), 0, 1, state.randomFloat);
            //state.raytracer.setWorld(state.bvh.get());
            startRaytracer(state);
        }
        ImGui::SameLine(140);
        if (ImGui::Button("Clear objects"))
        {
            clearObjects(state);
        }
        ImGui::EndChild();
    }

    void startRaytracer(const GuiState &state)
    {
        /*
        auto invokeRaytraceRender = [](Raytracer& raytracer, int samples)
        {
            raytracer.render(samples);
        };
        auto th = std::thread(invokeRaytraceRender, std::ref(state.raytracer), state.renderSamples);
        th.detach();
        */
    }

    void rightSideBarResize(GuiState &state)
    {
        ImVec2 hover_min(static_cast<float>(state.windowWidth) - state.rightSideBarWidth, 0);
        ImVec2 hover_max(
                static_cast<float>(state.windowWidth) - state.rightSideBarWidth + state.rightSideBarHoverMargin,
                static_cast<float>(state.windowHeight));
        resizeWindow(hover_min, hover_max, state.rightSideBarWidth, state.rightSideBarMinWidth,
                     state.rightSideBarMaxWidth,
                     state.isRightSideBarResizing, orientation::HORIZONTAL);
    }

    void resizeWindow(const ImVec2 &hover_min, const ImVec2 &hover_max, float &resize_pos,
                      const float &resize_pos_min, const float &resize_pos_max, bool &is_resizing,
                      const orientation &resize_orientation)
    {
        ImGuiMouseCursor cursor_type;
        switch (resize_orientation)
        {
            case (orientation::HORIZONTAL) :
                cursor_type = ImGuiMouseCursor_ResizeEW;
                break;
            case (orientation::VERTICAL) :
                cursor_type = ImGuiMouseCursor_ResizeNS;
                break;
        }
        if (ImGui::IsMouseHoveringRect(hover_min, hover_max, false))
        {
            ImGui::SetMouseCursor(cursor_type);
            if (ImGui::IsMouseClicked(0))
            {
                is_resizing = true;
            }
        }
        if (is_resizing)
        {
            ImGui::SetMouseCursor(cursor_type);
            if (ImGui::IsMouseReleased(0))
            {
                is_resizing = false;
            }
            ImGui::SetMouseCursor(cursor_type);
            float amount;
            if (resize_orientation == orientation::HORIZONTAL)
            {
                amount = ImGui::GetMouseDragDelta(0).x * (-1);
            } else
            {
                amount = ImGui::GetMouseDragDelta(0).y * (-1);
            }
            ImGui::ResetMouseDragDelta();
            if (resize_pos + amount >= resize_pos_max)
            {
                resize_pos = resize_pos_max;
            } else if (resize_pos + amount <= resize_pos_min)
            {
                resize_pos = resize_pos_min;
            } else
            {
                resize_pos += amount;
            }
        }
    }

    void moveTextureWhenDragged(GuiState &state)
    {
        if (ImGui::IsMouseDragging(0))
        {
            ImVec2 mouse_delta = ImGui::GetMouseDragDelta();
            ImGui::ResetMouseDragDelta();
            mouse_delta.x = mouse_delta.x;
            mouse_delta.y = mouse_delta.y;
            state.textureOffset.x += mouse_delta.x;
            state.textureOffset.y += mouse_delta.y;
        }
    }

    void zoomTextureWhenScrolled(GuiState &state)
    {
        float zoom = 1 + ImGui::GetIO().MouseWheel / 10;
        if (zoom != 0 && state.textureHeight * zoom > 0 && state.textureWidth * zoom > 0)
        {
            state.textureWidth *= zoom;
            state.textureHeight *= zoom;
        }
    }

    void randomizeWorld(const int &spheres, const int &scatter, GuiState &state)
    {
        // Use old demo scene from cli.cpp
        std::unique_ptr<Material> matptr = std::make_unique<Mix>(Vector3D(0.3, 0.3, 0.8), Vector3D(0.2, 1, 0.1), 0.1,
                                                                 0.1, 1, 1);
        std::unique_ptr<Material> lambptr = std::make_unique<Mix>(Vector3D(0.3, 0.3, 0.3), Vector3D(0.1, 0.1, 0.1), 0,
                                                                  0, 0.1, 1);
        std::unique_ptr<Material> redptr = std::make_unique<Lambertian>(Vector3D(1, 0.2, 0.2));
        std::unique_ptr<Material> glassptr = std::make_unique<Dielectric>(1.5);
        std::unique_ptr<Material> closeptr = std::make_unique<Mix>(Vector3D(0.0, 0.3, 0.3), Vector3D(1, 1, 1), 0.1, 0,
                                                                   1, 1);

        std::unique_ptr<Material> *mats[] = {&matptr, &lambptr, &redptr, &glassptr, &closeptr};

        for (int i = 0; i < spheres; ++i)
        {
            std::shared_ptr<Sphere> sphere;
            const float randi = state.randomFloat();
            if (randi < 0.25)
            {
                sphere = std::make_shared<Sphere>(
                        Vector3D(scatter * state.randomFloat() - scatter / 2, 0.1, -scatter * state.randomFloat()), 0.1,
                        glassptr.get());
            } else if (randi < 0.5)
            {
                sphere = std::make_shared<Sphere>(
                        Vector3D(scatter * state.randomFloat() - scatter / 2, 0.1, -scatter * state.randomFloat()), 0.1,
                        redptr.get());
            } else
            {
                sphere = std::make_shared<Sphere>(
                        Vector3D(scatter * state.randomFloat() - scatter / 2, 0.1, -scatter * state.randomFloat()), 0.1,
                        matptr.get());
            }
            state.world.push_back(sphere);
        }

        // Push pre-defined large spheres
        state.world.push_back(std::make_shared<Sphere>(Vector3D(0, -1000, 0), 1000, lambptr.get()));
        state.world.push_back(std::make_shared<Sphere>(Vector3D(0, 0.5, -1), 0.5, closeptr.get()));
        state.world.push_back(std::make_shared<Sphere>(Vector3D(-10, 4.7, -20), 5, matptr.get()));
        state.world.push_back(std::make_shared<Sphere>(Vector3D(-1.5, 0.5, -3), 0.5, closeptr.get()));
        state.world.push_back(std::make_shared<Sphere>(Vector3D(-1.5, 15, -3), 10, lambptr.get()));

        for (size_t i = 0; i < 5; i++)
        {
            state.world_materials.push_back(std::move((*mats[i])));
        }
    }

    void clearObjects(GuiState &state)
    {
        state.world.clear();
        state.hittableNames.clear();
        state.world_materials.clear();
    }
}