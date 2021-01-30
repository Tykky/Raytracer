#include <GL/glew.h>
#include "Gui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <random>
#include <hittables/Sphere.h>
#include <string>
#include <chrono>
#include <algorithm>
#include "hittables/Bvhnode.h"
#include "hittables/Sphere.h"
#include "hittables/Triangle.h"
#include "materials/Mix.h"
#include "materials/Lambertian.h"
#include "materials/Dielectric.h"
#include "materials/Metal.h"
#include "style.h"

Gui::Gui(GLFWwindow *window) :
    window_(window),
    render_width_(2560),
    render_height_(1440),
    render_samples_(10),

    display_imgui_metrics_(false),
    display_imgui_demo_(false),
    display_imgui_about_(false),
    display_imgui_userguide_(false),
    display_save_as_(false),
    display_menu_debug_(true),
    display_menu_file_(true),
    display_menu_window_(true), 
    main_menubar_height_(19),   
    texture_offset_(ImVec2(0, 0)),
    right_side_bar_width_(300),
    right_side_bar_min_width_(10),
    right_side_bar_max_width_(300),
    right_side_bar_hover_margin_(10),
    perf_monitor_height_(120),
    perf_monitor_graph_data_(),
    perf_monitor_resolution_(1000),
    samples_per_second_(0),
    is_right_side_bar_resizing_(false), 
    static_window_flags_(ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoBringToFrontOnFocus),   
    camera_(),
    camera_pos_x_(0),
    camera_pos_y_(0),
    camera_pos_z_(0),
    camera_up_x_(0),
    camera_up_y_(1),
    camera_up_z_(0),
    camera_fov_(90),    
    current_hittable(0),
    current_hittable_name(),
    current_hittable_pos_x_(0),
    current_hittable_pos_y_(0),
    current_hittable_pos_z_(0),
    current_sphere_radius_(1),
    randomizer_sphere_count_(100),
    randomizer_scatter_multiplier_(25),
    current_material_(0),    
    hittable_names_(),
    material_names({
        "Lambertian",
        "Metal",
        "Dielectric"
    }),
    world_(),
    world_materials_(std::vector<std::unique_ptr<Material>>()),   
    lambertian_(std::make_unique<Lambertian>(Vector3D(0.5f,0.5f,0.5f))),
    metal_(std::make_unique<Metal>(Vector3D(0.8,0.6,0.4), 0.3f)),
    dielectric_(std::make_unique<Dielectric>(1.3f)), 
    generator(std::mt19937()),
    dist(std::uniform_real_distribution<float>(0,1)),
    randomFloat(std::bind(dist, generator)),    
    raytracer_( nullptr, &camera_, render_width_, render_height_),  
    framebuffer_texture_id_(0), 
    file_submenu_({
        {"Save as", &display_save_as_}
    }), 
    debug_submenu_({
        {"ImGui metrics",    &display_imgui_metrics_},
        {"ImGui demo",       &display_imgui_demo_},
        {"ImGui about",      &display_imgui_about_},
        {"ImGui user guide", &display_imgui_userguide_},
    }), 
    window_submenu_({}),
    mainmenu_({
        {"File",   &display_menu_file_,   &file_submenu_},
        //{"Debug",  &display_menu_debug_,  &debug_submenu_}
    }) {
        
    texture_width_ = static_cast<float>(render_width_);
    texture_height_ = static_cast<float>(render_height_);
    perf_monitor_graph_data_ = std::vector<float>(perf_monitor_resolution_);

    // Stuff used for debugging (WIP)
    std::unique_ptr<Material> mat = std::make_unique<Lambertian>(Vector3D(0.5, 0.5, 0.5));
    std::unique_ptr<Hittable> tri = std::make_unique<Triangle>(Vector3D(1,-1,-2), Vector3D(1,-1,2), Vector3D(3,0.5,0), mat.get());
    world_materials_.push_back(std::move(mat));
    world_.push_back(std::move(tri));
}

Gui::~Gui() {
    ImGui::DestroyContext();
}

void Gui::init() {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    framebuffer_texture_id_ = setupTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width_, render_height_, 0, GL_RGB, GL_UNSIGNED_BYTE, raytracer_.getFramebuffer().data());
    execImGuiStyle();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().ChildRounding = 0.0f;
    ImGui::GetStyle().FrameRounding = 0.0f;
    ImGui::GetStyle().GrabRounding = 0.0f;
    ImGui::GetStyle().PopupRounding = 0.0f;
    ImGui::GetStyle().ScrollbarRounding = 0.0f;
}

void Gui::renderGui() {
    const auto time_start = std::chrono::high_resolution_clock::now();
    const uint64_t samples_start = raytracer_.getSampleCounter();

    glfwGetWindowSize(window_, &window_width_, &window_height_);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    rightSideBarResize();

    displayMainMenu();
    displayRenderedImage();
    displayRightSideBar();
    displayPerfMonitor();

    if (display_imgui_metrics_) ImGui::ShowMetricsWindow();
    if (display_imgui_demo_) ImGui::ShowDemoWindow();
    if (display_imgui_about_) ImGui::ShowAboutWindow();
    if (display_imgui_userguide_) ImGui::ShowUserGuide();
    if (display_save_as_) displaySaveAs();

    ImGui::Render();

    const uint64_t samples_end = raytracer_.getSampleCounter();
    const auto time_end = std::chrono::high_resolution_clock::now();

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();
    double samples_delta = static_cast<double>(samples_end - samples_start);
    samples_per_second_ = static_cast<float>((samples_delta / duration) * 1000000);
}

void Gui::renderDrawData() const {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::displayMainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        for (const Menuitem &item : mainmenu_) {
            if (item.display && ImGui::BeginMenu(item.label.c_str())) {
                for (const Menuitem &subitem : *item.submenu) {
                    if (ImGui::MenuItem(subitem.label.c_str(), nullptr, *subitem.display)) {
                        *subitem.display ^= true;
                    }
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }
}

unsigned int Gui::setupTexture() const {
    unsigned int texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}

void Gui::displaySaveAs() {
    ImGui::Begin("Save as", &display_save_as_);
    ImGui::Text("Save rendered image to .ppm file");
    const unsigned int size = 512;
    char filename[size] = {};
    ImGui::InputText("filename", filename, size);
    if (ImGui::Button("Save")) {
        std::string fname = filename;
        raytracer_.frammebufferToNetpbm(fname);
    }
    ImGui::End();
}

void Gui::displayRenderedImage() {
    ImGui::SetNextWindowPos(ImVec2(0, main_menubar_height_));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width_) - right_side_bar_width_, static_cast<float>(window_height_) - perf_monitor_height_));
    ImGui::Begin("Rendered image", nullptr, static_window_flags_ | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImVec2 window_size = ImGui::GetWindowSize();

    if (ImGui::IsWindowHovered()) {
        moveTextureWhenDragged();
        zoomTextureWhenScrolled();
    }

    ImVec2 texture_center = ImVec2((window_size.x - texture_width_) * 0.5f + texture_offset_.x,
                                   (window_size.y - texture_height_) * 0.5f + texture_offset_.y);

    ImGui::SetCursorPos(texture_center);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, render_width_, render_height_,
                    GL_RGB, GL_UNSIGNED_BYTE, raytracer_.getFramebuffer().data());

    ImGui::Image((void*)(intptr_t)framebuffer_texture_id_, ImVec2(texture_width_, texture_height_));
    ImGui::End();
}

void Gui::displayRightSideBar() {
    ImGui::SetNextWindowSize(ImVec2(right_side_bar_width_, static_cast<float>(window_height_) - main_menubar_height_));
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(window_width_) - right_side_bar_width_, main_menubar_height_));
    ImGui::Begin("right_side_bar", nullptr, static_window_flags_);

    if (ImGui::IsWindowHovered()) {
        ImGui::SetWindowFocus();
    }
    displayRenderSettingsChild(ImVec2(right_side_bar_max_width_, 120));
    displayCameraSettingsChild(ImVec2(right_side_bar_max_width_, 120));
    displayRandomizerChild(ImVec2(right_side_bar_width_, 150));
    displayObjectsChild(ImVec2(right_side_bar_width_, 300));
    ImGui::End();
}

void Gui::displayRenderSettingsChild(const ImVec2 &size) {
    ImGui::BeginChild("render_settings", size, true);
    ImGui::Text("Render settings");
    int *res[] = {&render_width_, &render_height_};
    if (ImGui::InputInt2("Resolution", *res)) {
        raytracer_.resize(render_width_, render_height_);
        raytracer_.clearFramebuffer();
    }
    ImGui::InputInt("Samples", &render_samples_);
    if (ImGui::Button("Render") && !world_.empty() && !raytracer_.isRendering()) {
        bvh = std::make_shared<Bvhnode>(world_,0, world_.size(), 0, 1, randomFloat);
        raytracer_.setWorld(bvh.get());
        startRaytracer();
    }
    ImGui::SameLine(65);
    if (ImGui::Button("Clear")) {
        raytracer_.clearFramebuffer();
    }
    if (ImGui::Button("Render randomized") && !raytracer_.isRendering()) {
        world_.clear();
        world_materials_.clear();
        randomizeWorld(randomizer_sphere_count_, randomizer_scatter_multiplier_);
        bvh = std::make_shared<Bvhnode>(world_,0, world_.size(), 0, 1, randomFloat);
        raytracer_.setWorld(bvh.get());
        startRaytracer();
    }
    ImGui::EndChild();
}

void Gui::displayCameraSettingsChild(const ImVec2 &size) {
    ImGui::BeginChild("camera_Settings", size, true);
    ImGui::Text("Camera settings");
    float *pos[] = {&camera_pos_x_, &camera_pos_y_, &camera_pos_z_};
    if (ImGui::InputFloat3("Position", *pos)) {
        camera_.setPos({camera_pos_x_, camera_pos_y_, camera_pos_z_});
        camera_.applyChanges();
    }
    float *up[] = {&camera_up_x_, &camera_up_y_, &camera_pos_z_};
    if (ImGui::InputFloat3("Up pos", *up)) {
        camera_.setUp({camera_up_x_, camera_up_y_, camera_up_z_});
        camera_.applyChanges();
    }
    if (ImGui::SliderFloat("Fov", &camera_fov_, 0, 110)) {
        camera_.setFov(camera_fov_);
        camera_.applyChanges();
    }
    ImGui::EndChild();
}

void Gui::displayObjectsChild(const ImVec2 &size) {
    ImGui::BeginChild("add_obj", size, true);
    ImGui::Text("Add spheres");
    ImGui::ListBox("", &current_hittable, hittable_names_.data(), hittable_names_.size(), 5);
    const int buf_size = 20;
    ImGui::InputText("Name", current_hittable_name, buf_size);
    ImGui::Combo("Material", &current_material_, material_names.data(), material_names.size());
    float *pos[] = { &current_hittable_pos_x_, &current_hittable_pos_y_, &current_hittable_pos_z_ };
    ImGui::InputFloat3("Position ", *pos);
    ImGui::InputFloat("Radius", &current_sphere_radius_);
    if (ImGui::Button("Add") && strlen(current_hittable_name) > 0) {
        char *text = new char[buf_size];
        strcpy(text, current_hittable_name);
        hittable_names_.push_back(text);
        Material *mat;
        if (current_material_ == 0) {
            mat = lambertian_.get();
        } else if (current_material_ == 1) {
            mat = metal_.get();
        } else if (current_material_ == 2) {
            mat = dielectric_.get();
        }
        world_.push_back(std::make_shared<Sphere>(Vector3D(current_hittable_pos_x_, current_hittable_pos_y_, current_hittable_pos_z_), current_sphere_radius_, mat));
    }
    ImGui::SameLine(40);
    if (ImGui::Button("Delete") && current_hittable < hittable_names_.size()) {
        delete[] hittable_names_.at(current_hittable);
        hittable_names_.erase(hittable_names_.begin() + current_hittable);
        world_.erase(world_.begin() + current_hittable);
    }
    ImGui::EndChild();
}

void Gui::displayRandomizerChild(const ImVec2 &size) {
    ImGui::BeginChild("randomizer", size, true);
    ImGui::Text("Randomizer settings");
    ImGui::InputInt("Spheres", &randomizer_sphere_count_);
    ImGui::InputInt("Scatter", &randomizer_scatter_multiplier_);
    ImGui::EndChild();
}

void Gui::displayPerfMonitor() {
    perf_monitor_graph_data_.push_back(samples_per_second_);
    perf_monitor_graph_data_.erase(perf_monitor_graph_data_.begin());
    const size_t display_size = 1000;

    ImGui::Begin("perfmonitor", nullptr, static_window_flags_);
    ImGui::Text(("Samples (M) per second " + std::to_string(samples_per_second_ / 1000000)).data());
    ImGui::SameLine(250);
    ImGui::Text("Status:");
    ImGui::SameLine(300);
    if (raytracer_.isRendering()) {
        ImGui::TextColored(ImVec4(0,1,0,1), "Rendering");
    } else {
        ImGui::TextColored(ImVec4(1,0,0,1), "Stopped");
    }
    const float width = window_width_ - right_side_bar_width_;
    ImGui::SetWindowPos(ImVec2(0, window_height_ - perf_monitor_height_ + main_menubar_height_));
    ImGui::SetWindowSize(ImVec2(width ,perf_monitor_height_));
    const float *graph_data_ptr = perf_monitor_graph_data_.data() + perf_monitor_graph_data_.size() - display_size;
    ImGui::PlotLines("lines", graph_data_ptr, display_size, 0, "", FLT_MAX, FLT_MAX, ImVec2(width, perf_monitor_height_ - 50));
    ImGui::End();
}

void Gui::startRaytracer() {
    auto invokeRaytraceRender = [](Raytracer &raytracer, int samples) {
        raytracer.render(samples);
    };
    auto th = std::thread(invokeRaytraceRender, std::ref(raytracer_), render_samples_);
    th.detach();
}

void Gui::rightSideBarResize() {
    ImVec2 hover_min(static_cast<float>(window_width_) - right_side_bar_width_, 0);
    ImVec2 hover_max(static_cast<float>(window_width_) - right_side_bar_width_ + right_side_bar_hover_margin_, static_cast<float>(window_height_));
    resizeWindow(hover_min, hover_max, right_side_bar_width_, right_side_bar_min_width_, right_side_bar_max_width_,
                 is_right_side_bar_resizing_, orientation::HORIZONTAL);
}

void Gui::resizeWindow(const ImVec2 &hover_min, const ImVec2 &hover_max, float &resize_pos,
                       const float &resize_pos_min, const float &resize_pos_max, bool &is_resizing,
                       const orientation &resize_orientation) 
    {
    ImGuiMouseCursor cursor_type;
    switch(resize_orientation) {
        case(orientation::HORIZONTAL) :
            cursor_type = ImGuiMouseCursor_ResizeEW;
            break;
        case(orientation::VERTICAL) :
            cursor_type = ImGuiMouseCursor_ResizeNS;
            break;
    }
    if (ImGui::IsMouseHoveringRect(hover_min, hover_max, false)) {
        ImGui::SetMouseCursor(cursor_type);
        if (ImGui::IsMouseClicked(0)) {
            is_resizing = true;
        }
    }
    if (is_resizing) {
        ImGui::SetMouseCursor(cursor_type);
        if (ImGui::IsMouseReleased(0)) {
            is_resizing = false;
        }
        ImGui::SetMouseCursor(cursor_type);
        float amount;
        if (resize_orientation == orientation::HORIZONTAL) {
            amount = ImGui::GetMouseDragDelta(0).x * (-1);
        } else {
            amount = ImGui::GetMouseDragDelta(0).y * (-1);
        }
        ImGui::ResetMouseDragDelta();
        if (resize_pos + amount >= resize_pos_max) {
            resize_pos = resize_pos_max;
        } else if (resize_pos + amount <= resize_pos_min) {
            resize_pos = resize_pos_min;
        } else {
            resize_pos += amount;
        }
    }
}

void Gui::moveTextureWhenDragged() {
    if (ImGui::IsMouseDragging(0)) {
        ImVec2 mouse_delta = ImGui::GetMouseDragDelta();
        ImGui::ResetMouseDragDelta();
        mouse_delta.x = mouse_delta.x;
        mouse_delta.y = mouse_delta.y;
        texture_offset_.x += mouse_delta.x;
        texture_offset_.y += mouse_delta.y;
    }
}

void Gui::zoomTextureWhenScrolled() {
    float zoom = 1 + ImGui::GetIO().MouseWheel/10;
    if (zoom != 0 && texture_height_ * zoom > 0 && texture_width_ * zoom > 0) {
        texture_width_ *= zoom;
        texture_height_ *= zoom;
    }
}

void Gui::randomizeWorld(const int &spheres, const int &scatter) {
    // Use old demo scene from cli.cpp
    std::unique_ptr<Material> matptr = std::make_unique<Mix>(Vector3D(0.3, 0.3, 0.8), Vector3D(0.2, 1, 0.1), 0.1, 0.1, 1, 1);
    std::unique_ptr<Material> lambptr = std::make_unique<Mix>(Vector3D(0.3, 0.3, 0.3), Vector3D(0.1, 0.1, 0.1), 0, 0, 0.1, 1);
    std::unique_ptr<Material> redptr = std::make_unique<Lambertian>(Vector3D(1, 0.2, 0.2));
    std::unique_ptr<Material> glassptr = std::make_unique<Dielectric>(1.5);
    std::unique_ptr<Material> closeptr = std::make_unique<Mix>(Vector3D(0.0, 0.3, 0.3), Vector3D(1, 1, 1), 0.1, 0, 1, 1);

    std::unique_ptr<Material>* mats[] = { &matptr, &lambptr, &redptr, &glassptr, &closeptr };

    for (int i = 0; i < spheres; ++i) {
        std::shared_ptr<Sphere> sphere;
        const float randi = randomFloat();
        if (randi < 0.25) {
            sphere = std::make_shared<Sphere>(Vector3D(scatter * randomFloat() - scatter / 2, 0.1, - scatter * randomFloat()), 0.1, glassptr.get());
        } else if (randi < 0.5) {
            sphere = std::make_shared<Sphere>(Vector3D(scatter * randomFloat() - scatter / 2, 0.1, - scatter * randomFloat()), 0.1, redptr.get());
        } else {
            sphere = std::make_shared<Sphere>(Vector3D(scatter * randomFloat() - scatter / 2, 0.1, - scatter * randomFloat()), 0.1, matptr.get());
        }
        world_.push_back(sphere);
    }

    // Push pre-defined large spheres
    world_.push_back(std::make_shared<Sphere>(Vector3D(0, -1000, 0), 1000, lambptr.get()));

    world_.push_back(std::make_shared<Sphere>(Vector3D(0, 0.5, -1), 0.5, closeptr.get()));
    world_.push_back(std::make_shared<Sphere>(Vector3D(-10, 4.7, -20), 5, matptr.get()));
    world_.push_back(std::make_shared<Sphere>(Vector3D(-1.5, 0.5, -3), 0.5, closeptr.get()));
    world_.push_back(std::make_shared<Sphere>(Vector3D(-1.5, 15, -3), 10, lambptr.get()));

    for (size_t i = 0; i < 5; i++) {
        world_materials_.push_back(std::move((*mats[i])));
    }
}