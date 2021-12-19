#include <GL/glew.h>
#include "Gui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <random>
#include <hittables/Sphere.h>
#include <string>
#include <chrono>
#include <algorithm>
#include "hittables/Bvhnode.h"
#include "hittables/Triangle.h"
#include "io/meshloader.h"
#include "materials/Mix.h"
#include "materials/Lambertian.h"
#include "materials/Dielectric.h"
#include "style.h"

Gui::Gui(GLFWwindow *window) : m_window(window) 
{
    m_textureWidth = static_cast<float>(m_renderWidth);
    m_textureHeight = static_cast<float>(m_renderHeight);
    m_perfMonitorGraphData = std::vector<float>(m_PerfMonitorResolution);
}

Gui::~Gui() 
{
    ImGui::DestroyContext();
}

void Gui::init() 
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    m_framebufferTextureId = setupTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_renderWidth, m_renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_raytracer.getFramebuffer().data());
    execImGuiStyle();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().ChildRounding = 0.0f;
    ImGui::GetStyle().FrameRounding = 0.0f;
    ImGui::GetStyle().GrabRounding = 0.0f;
    ImGui::GetStyle().PopupRounding = 0.0f;
    ImGui::GetStyle().ScrollbarRounding = 0.0f;
}

void Gui::renderGui() 
{
    const auto time_start = std::chrono::high_resolution_clock::now();
    const uint64_t samples_start = m_raytracer.getSampleCounter();

    glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    rightSideBarResize();

    displayMainMenu();
    displayRenderedImage();
    displayRightSideBar();
    displayPerfMonitor();

    if (m_displayImguiMetrics)    ImGui::ShowMetricsWindow();
    if (m_displayImguiDemo)       ImGui::ShowDemoWindow();
    if (m_displayImguiAbout)      ImGui::ShowAboutWindow();
    if (m_displayImguiUserGUide)  ImGui::ShowUserGuide();
    if (m_displaySaveAs)          displaySaveAs();

    ImGui::Render();

    const uint64_t samples_end = m_raytracer.getSampleCounter();
    const auto time_end = std::chrono::high_resolution_clock::now();

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();
    auto samples_delta = static_cast<double>(samples_end - samples_start);
    m_SamplesPerSecond = static_cast<float>((samples_delta / duration) * 1000000);
}

void Gui::renderDrawData() const 
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::displayMainMenu() 
{
    if (ImGui::BeginMainMenuBar()) 
    {
        for (const Menuitem &item : m_mainmenu) 
        {
            if (item.display && ImGui::BeginMenu(item.label.c_str())) 
            {
                for (const Menuitem &subitem : *item.submenu) 
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

unsigned int Gui::setupTexture() const 
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

void Gui::displaySaveAs() 
{
    ImGui::Begin("Save as", &m_displaySaveAs);
    ImGui::Text("Save rendered image to .ppm file");
    const unsigned int size = 512;
    char filename[size] = {};
    ImGui::InputText("filename", filename, size);
    if (ImGui::Button("Save")) 
    {
        std::string fname = filename;
        std::cout << fname << std::endl;
        m_raytracer.frammebufferToNetpbm(fname);
    }
    ImGui::End();
}

void Gui::displayRenderedImage() 
{
    ImGui::SetNextWindowPos(ImVec2(0, m_mainMenubarHeight));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(m_windowWidth) - m_rightSideBarWidth, static_cast<float>(m_windowHeight) - m_perfMonitorHeight));
    ImGui::Begin("Rendered image", nullptr, m_staticWindowFlags | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImVec2 window_size = ImGui::GetWindowSize();

    if (ImGui::IsWindowHovered()) 
    {
        moveTextureWhenDragged();
        zoomTextureWhenScrolled();
    }

    ImVec2 texture_center = ImVec2((window_size.x - m_textureWidth) * 0.5f + m_textureOffset.x,
                                   (window_size.y - m_textureHeight) * 0.5f + m_textureOffset.y);

    ImGui::SetCursorPos(texture_center);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_renderWidth, m_renderHeight,
                    GL_RGB, GL_UNSIGNED_BYTE, m_raytracer.getFramebuffer().data());

    ImGui::Image((void*)(intptr_t)m_framebufferTextureId, ImVec2(m_textureWidth, m_textureHeight));
    ImGui::End();
}

void Gui::displayRightSideBar() 
{
    ImGui::SetNextWindowSize(ImVec2(m_rightSideBarWidth, static_cast<float>(m_windowHeight) - m_mainMenubarHeight));
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(m_windowWidth) - m_rightSideBarWidth, m_mainMenubarHeight));
    ImGui::Begin("right_side_bar", nullptr, m_staticWindowFlags);

    if (ImGui::IsWindowHovered()) 
    {
        ImGui::SetWindowFocus();
    }
    displayRenderSettingsChild(ImVec2(m_RightSideBarMaxWidth, 120));
    displayCameraSettingsChild(ImVec2(m_RightSideBarMaxWidth, 120));
    displayRandomizerChild(ImVec2(m_rightSideBarWidth, 150));
    displayObjectsChild(ImVec2(m_rightSideBarWidth, 300));
    ImGui::End();
}

void Gui::displayRenderSettingsChild(const ImVec2 &size) 
{
    ImGui::BeginChild("render_settings", size, true);
    ImGui::Text("Render settings");
    int *res[] = {&m_renderWidth, &m_renderHeight};
    if (ImGui::InputInt2("Resolution", *res)) 
    {
        m_raytracer.resize(m_renderWidth, m_renderHeight);
        m_raytracer.clearFramebuffer();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_renderWidth, m_renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_raytracer.getFramebuffer().data());
    }
    ImGui::InputInt("Samples", &m_renderSamples);
    if (ImGui::Button("Render") && !m_world.empty() && !m_raytracer.isRendering()) 
    {
        m_bvh = std::make_shared<Bvhnode>(m_world,0, m_world.size(), 0, 1, randomFloat);
        m_raytracer.setWorld(m_bvh.get());
        startRaytracer();
    }
    ImGui::SameLine(65);
    if (ImGui::Button("Clear")) 
    {
        m_raytracer.clearFramebuffer();
    }
    ImGui::EndChild();
}

void Gui::displayCameraSettingsChild(const ImVec2 &size) 
{
    ImGui::BeginChild("camera_Settings", size, true);
    ImGui::Text("Camera settings");
    float *pos[] = {&m_cameraPosX, &m_cameraPosY, &m_cameraPosZ};
    if (ImGui::InputFloat3("Position", *pos)) 
    {
        m_camera.setPos({m_cameraPosX, m_cameraPosY, m_cameraPosZ});
        m_camera.applyChanges();
    }
    float *up[] = {&m_cameraUpX, &m_cameraUpY, &m_cameraPosZ};
    if (ImGui::InputFloat3("Up pos", *up)) 
    {
        m_camera.setUp({m_cameraUpX, m_cameraUpY, m_cameraUpZ});
        m_camera.applyChanges();
    }
    if (ImGui::SliderFloat("Fov", &m_cameraFov, 0, 110)) 
    {
        m_camera.setFov(m_cameraFov);
        m_camera.applyChanges();
    }
    ImGui::EndChild();
}

void Gui::displayObjectsChild(const ImVec2 &size) 
{
    ImGui::BeginChild("add_obj", size, true);
    ImGui::Text("Add objects");
    ImGui::ListBox("", &m_currentHittable, m_hittableNames.data(), m_hittableNames.size(), 5);
    const int buf_size = 20;
    ImGui::InputText("Name", m_currentHittableName, buf_size);
    ImGui::Combo("Obj type", &m_currrentObjectType, m_objectTypes.data(), m_objectTypes.size());
    ImGui::Combo("Material", &m_currentMaterial, m_materialNames.data(), m_materialNames.size());
    float *pos[] = { &m_currentHittablePosX, &m_currentHittablePosY, &m_currentHittablePosZ };
    ImGui::InputFloat3("Position ", *pos);
    if (m_currrentObjectType == 0) 
    {
        ImGui::InputFloat("Radius", &m_currentSphereRadius);
    } 
    else if (m_currrentObjectType == 1) 
    {
        ImGui::InputFloat3("Vertex 0 ", m_currentVertex0);
        ImGui::InputFloat3("Vertex 1 ", m_currentVertex1);
        ImGui::InputFloat3("Vertex 2 ", m_currentVertex2);
        ImGui::InputFloat3("normal", m_currentNormal);
    } 
    else 
    {
        ImGui::InputText("Filename", m_currentObjFilename, 100);
    }
    if (ImGui::Button("Add") && strlen(m_currentHittableName) > 0) 
    {
        char *text = new char[buf_size];
        strcpy(text, m_currentHittableName);
        m_hittableNames.push_back(text);
        Material *mat;
        std::shared_ptr<Primitive> object;
        if (m_currentMaterial == 0) 
        {
            mat = m_lambertian.get();
        } 
        else if (m_currentMaterial == 1) 
        {
            mat = m_metal.get();
        } 
        else if (m_currentMaterial == 2) 
        {
            mat = m_dielectric.get();
        }
        if (m_currrentObjectType == 0) 
        {
            object = std::make_shared<Sphere>(Vector3D(m_currentHittablePosX, m_currentHittablePosY, m_currentHittablePosZ), m_currentSphereRadius, mat);
        } 
        else if (m_currrentObjectType == 1) 
        {
            object = std::make_shared<Triangle>(Vector3D(m_currentVertex0[0], m_currentVertex0[1], m_currentVertex0[2]),
                                                Vector3D(m_currentVertex1[0], m_currentVertex1[1], m_currentVertex1[2]),
                                                Vector3D(m_currentVertex2[0], m_currentVertex1[1], m_currentVertex2[2]),
                                                Vector3D(m_currentNormal[0], m_currentNormal[1], m_currentNormal[2]), mat);
        } 
        else 
        {
            object = loadObj(m_currentObjFilename, mat, randomFloat)[0]; // Load only the first object for now
            Mesh *objptr = dynamic_cast<Mesh *>(object.get());
            objptr->setOffset(Vector3D(m_currentHittablePosX, m_currentHittablePosY, m_currentHittablePosZ));
        }
        m_world.push_back(object);
    }
    ImGui::SameLine(40);
    if (ImGui::Button("Delete") && m_currentHittable < m_hittableNames.size()) 
    {
        delete[] m_hittableNames.at(m_currentHittable);
        m_hittableNames.erase(m_hittableNames.begin() + m_currentHittable);
        m_world.erase(m_world.begin() + m_currentHittable);
    }
    ImGui::EndChild();
}

void Gui::displayRandomizerChild(const ImVec2 &size) 
{
    ImGui::BeginChild("randomizer", size, true);
    ImGui::Text("Randomizer settings");
    ImGui::InputInt("Spheres", &m_randomizerSphereCOunt);
    ImGui::InputInt("Scatter", &m_randomizerScatterMultiplier);
    if (ImGui::Button("Render randomized") && !m_raytracer.isRendering()) 
    {
        clearObjects();
        randomizeWorld(m_randomizerSphereCOunt, m_randomizerScatterMultiplier);
        m_bvh = std::make_shared<Bvhnode>(m_world,0, m_world.size(), 0, 1, randomFloat);
        m_raytracer.setWorld(m_bvh.get());
        startRaytracer();
    }
    ImGui::SameLine(140);
    if (ImGui::Button("Clear objects")) 
    {
        clearObjects();
    }
    ImGui::EndChild();
}

void Gui::displayPerfMonitor() 
{
    m_perfMonitorGraphData.push_back(m_SamplesPerSecond);
    m_perfMonitorGraphData.erase(m_perfMonitorGraphData.begin());
    const size_t display_size = 1000;

    ImGui::Begin("perfmonitor", nullptr, m_staticWindowFlags);
    ImGui::Text(("Mega samples p/s " + std::to_string(m_SamplesPerSecond / 1000000)).data());
    ImGui::SameLine(250);
    ImGui::Text("Status:");
    ImGui::SameLine(300);
    if (m_raytracer.isRendering()) 
    {
        ImGui::TextColored(ImVec4(0,1,0,1), "Rendering");
    } 
    else 
    {
        ImGui::TextColored(ImVec4(1,0,0,1), "Stopped");
    }
    const float width = m_windowWidth - m_rightSideBarWidth;
    ImGui::SetWindowPos(ImVec2(0, m_windowHeight - m_perfMonitorHeight + m_mainMenubarHeight));
    ImGui::SetWindowSize(ImVec2(width ,m_perfMonitorHeight));
    const float *graph_data_ptr = m_perfMonitorGraphData.data() + m_perfMonitorGraphData.size() - display_size;
    ImGui::PlotLines("lines", graph_data_ptr, display_size, 0, "", FLT_MAX, FLT_MAX, ImVec2(width, m_perfMonitorHeight - 50));
    ImGui::End();
}

void Gui::startRaytracer() 
{
    auto invokeRaytraceRender = [](Raytracer& raytracer, int samples)
    {
        raytracer.render(samples);
    };
    auto th = std::thread(invokeRaytraceRender, std::ref(m_raytracer), m_renderSamples);
    th.detach();
}

void Gui::rightSideBarResize() 
{
    ImVec2 hover_min(static_cast<float>(m_windowWidth) - m_rightSideBarWidth, 0);
    ImVec2 hover_max(static_cast<float>(m_windowWidth) - m_rightSideBarWidth + m_rightSideBarHoverMargin, static_cast<float>(m_windowHeight));
    resizeWindow(hover_min, hover_max, m_rightSideBarWidth, m_rightSideBarMinWidth, m_RightSideBarMaxWidth,
                 m_isRightSideBarResizing, orientation::HORIZONTAL);
}

void Gui::resizeWindow(const ImVec2 &hover_min, const ImVec2 &hover_max, float &resize_pos,
                       const float &resize_pos_min, const float &resize_pos_max, bool &is_resizing,
                       const orientation &resize_orientation) 
{
    ImGuiMouseCursor cursor_type;
    switch(resize_orientation) 
    {
        case(orientation::HORIZONTAL) :
            cursor_type = ImGuiMouseCursor_ResizeEW;
            break;
        case(orientation::VERTICAL) :
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
        } 
        else 
        {
            amount = ImGui::GetMouseDragDelta(0).y * (-1);
        }
        ImGui::ResetMouseDragDelta();
        if (resize_pos + amount >= resize_pos_max) 
        {
            resize_pos = resize_pos_max;
        } 
        else if (resize_pos + amount <= resize_pos_min) 
        {
            resize_pos = resize_pos_min;
        } 
        else 
        {
            resize_pos += amount;
        }
    }
}

void Gui::moveTextureWhenDragged() 
{
    if (ImGui::IsMouseDragging(0)) 
    {
        ImVec2 mouse_delta = ImGui::GetMouseDragDelta();
        ImGui::ResetMouseDragDelta();
        mouse_delta.x = mouse_delta.x;
        mouse_delta.y = mouse_delta.y;
        m_textureOffset.x += mouse_delta.x;
        m_textureOffset.y += mouse_delta.y;
    }
}

void Gui::zoomTextureWhenScrolled() 
{
    float zoom = 1 + ImGui::GetIO().MouseWheel/10;
    if (zoom != 0 && m_textureHeight * zoom > 0 && m_textureWidth * zoom > 0) 
    {
        m_textureWidth *= zoom;
        m_textureHeight *= zoom;
    }
}

void Gui::randomizeWorld(const int &spheres, const int &scatter) 
{
    // Use old demo scene from cli.cpp
    std::unique_ptr<Material> matptr = std::make_unique<Mix>(Vector3D(0.3, 0.3, 0.8), Vector3D(0.2, 1, 0.1), 0.1, 0.1, 1, 1);
    std::unique_ptr<Material> lambptr = std::make_unique<Mix>(Vector3D(0.3, 0.3, 0.3), Vector3D(0.1, 0.1, 0.1), 0, 0, 0.1, 1);
    std::unique_ptr<Material> redptr = std::make_unique<Lambertian>(Vector3D(1, 0.2, 0.2));
    std::unique_ptr<Material> glassptr = std::make_unique<Dielectric>(1.5);
    std::unique_ptr<Material> closeptr = std::make_unique<Mix>(Vector3D(0.0, 0.3, 0.3), Vector3D(1, 1, 1), 0.1, 0, 1, 1);

    std::unique_ptr<Material>* mats[] = { &matptr, &lambptr, &redptr, &glassptr, &closeptr };

    for (int i = 0; i < spheres; ++i) 
    {
        std::shared_ptr<Sphere> sphere;
        const float randi = randomFloat();
        if (randi < 0.25) 
        {
            sphere = std::make_shared<Sphere>(Vector3D(scatter * randomFloat() - scatter / 2, 0.1, - scatter * randomFloat()), 0.1, glassptr.get());
        } 
        else if (randi < 0.5) 
        {
            sphere = std::make_shared<Sphere>(Vector3D(scatter * randomFloat() - scatter / 2, 0.1, - scatter * randomFloat()), 0.1, redptr.get());
        } 
        else 
        {
            sphere = std::make_shared<Sphere>(Vector3D(scatter * randomFloat() - scatter / 2, 0.1, - scatter * randomFloat()), 0.1, matptr.get());
        }
        m_world.push_back(sphere);
    }

    // Push pre-defined large spheres
    m_world.push_back(std::make_shared<Sphere>(Vector3D(0, -1000, 0), 1000, lambptr.get()));
    m_world.push_back(std::make_shared<Sphere>(Vector3D(0, 0.5, -1), 0.5, closeptr.get()));
    m_world.push_back(std::make_shared<Sphere>(Vector3D(-10, 4.7, -20), 5, matptr.get()));
    m_world.push_back(std::make_shared<Sphere>(Vector3D(-1.5, 0.5, -3), 0.5, closeptr.get()));
    m_world.push_back(std::make_shared<Sphere>(Vector3D(-1.5, 15, -3), 10, lambptr.get()));

    for (size_t i = 0; i < 5; i++) 
    {
        m_world_materials.push_back(std::move((*mats[i])));
    }
}

void Gui::clearObjects() 
{
    m_world.clear();
    m_hittableNames.clear();
    m_world_materials.clear();
}
