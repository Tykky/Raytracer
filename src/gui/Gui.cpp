#include <GL/glew.h>
#include "Gui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <random>
#include <hittables/Sphere.h>
#include <string>
#include "hittables/Bvhnode.h"
#include "hittables/Sphere.h"
#include "materials/Mix.h"

#define IMGUI_DEFINE_MATH_OPERATORS


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

        is_right_side_bar_resizing_(false),

        static_window_flags_(ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoBringToFrontOnFocus |
                             ImGuiWindowFlags_NoScrollWithMouse |
                             ImGuiWindowFlags_NoScrollbar),

        camera_(90,
               static_cast<float>(render_width_) / static_cast<float>(render_height_),
                Vector3D(0,0,0),
                Vector3D(1,0,0),
                Vector3D(0,1,0)),

        raytracer_(nullptr, &camera_, render_width_, render_height_),

        file_submenu_({
                              {"Save as", &display_save_as_}
                      }),



        framebuffer_texture_id_(0) {

    file_submenu_ = {
            {"Save as", &display_save_as_}
    };

    debug_submenu_ = {
            {"ImGui metrics",    &display_imgui_metrics_},
            {"ImGui demo",       &display_imgui_demo_},
            {"ImGui about",      &display_imgui_about_},
            {"ImGui user guide", &display_imgui_userguide_},
    };

    window_submenu_ = {
    };

    mainmenu_ = {
            {"File",   &display_menu_file_,   &file_submenu_},
            {"Window", &display_menu_window_, &window_submenu_},
            {"Debug",  &display_menu_debug_,  &debug_submenu_}
    };

    texture_width_ = static_cast<float>(render_width_);
    texture_height_ = static_cast<float>(render_height_);

}

Gui::~Gui() {
    ImGui::DestroyContext();
}

void Gui::displayMainMenu() {
    if(ImGui::BeginMainMenuBar()) {
        for(const Menuitem &item : mainmenu_) {
            if(item.display && ImGui::BeginMenu(item.label.c_str())) {
                for (const Menuitem &subitem : *item.submenu) {
                    if(ImGui::MenuItem(subitem.label.c_str(), nullptr, *subitem.display)) {
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
    ImGui::Button("Save");
    ImGui::End();
}

void Gui::displayRenderedImage() {
    ImGui::SetNextWindowPos(ImVec2(0, main_menubar_height_));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width_) - right_side_bar_width_, static_cast<float>(window_height_)));
    ImGui::Begin("Rendered image", nullptr, static_window_flags_);
    ImVec2 window_size = ImGui::GetWindowSize();

    if(ImGui::IsWindowFocused()) {
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
    ImGui::BeginChild("render_settings", ImVec2(right_side_bar_width_, 120), true);
    ImGui::Text("Render settings");
    int *res[] = {&render_width_, &render_height_};

    if(ImGui::InputInt2("Resolution", *res)) {
        raytracer_.resize(render_width_, render_height_);
        raytracer_.clearFramebuffer();
    }

    ImGui::InputInt("Samples", &render_samples_);
    if(ImGui::Button("Render")) {
        startRaytracer();
    }

    ImGui::SameLine(65);
    if(ImGui::Button("Clear")) {
        raytracer_.clearFramebuffer();
    }

    ImGui::EndChild();
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
    if(ImGui::IsMouseHoveringRect(hover_min, hover_max, false)) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        if(ImGui::IsMouseClicked(0)) {
            is_right_side_bar_resizing_ = true;
        }
    }
    if(is_right_side_bar_resizing_) {
        if(ImGui::IsMouseReleased(0)) {
            is_right_side_bar_resizing_ = false;
        }
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        float amount = ImGui::GetMouseDragDelta(0).x * (-1);
        ImGui::ResetMouseDragDelta();
        if(right_side_bar_width_ + amount > right_side_bar_max_width_) {
            right_side_bar_width_ = right_side_bar_max_width_;
        } else if(right_side_bar_width_ + amount < right_side_bar_min_width_) {
            right_side_bar_width_ = right_side_bar_min_width_;
        } else {
            right_side_bar_width_ += amount;
        }
    }
}

void Gui::resizeWindow(const ImVec2 &hover_min, const ImVec2 &hover_max, float &resize_pos,
                       const float &resize_pos_min, const float &resize_pos_max, bool &is_resizing,
                       const orientation &resize_orientation) {

    ImGuiMouseCursor cursor_type;
    switch(resize_orientation) {
        case(orientation::HORIZONTAL) :
            cursor_type = ImGuiMouseCursor_ResizeEW;
            break;
        case(orientation::VERTICAL) :
            cursor_type = ImGuiMouseCursor_ResizeNS;
            break;
    }
    if(ImGui::IsMouseHoveringRect(hover_min, hover_max, false)) {
        ImGui::SetMouseCursor(cursor_type);
        if(ImGui::IsMouseClicked(0)) {
            is_resizing = true;
        }
    }
    if(is_resizing) {
        ImGui::SetMouseCursor(cursor_type);
        if(ImGui::IsMouseReleased(0)) {
            is_resizing = false;
        }
        ImGui::SetMouseCursor(cursor_type);
        float amount = ImGui::GetMouseDragDelta(0).x * (-1);
        ImGui::ResetMouseDragDelta();
    }
}

void Gui::moveTextureWhenDragged() {
    if(ImGui::IsMouseDragging(0)) {
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
    if(zoom != 0 && texture_height_ * zoom > 0 && texture_width_ * zoom > 0) {
        texture_width_ *= zoom;
        texture_height_ *= zoom;
    }
}

void Gui::init() {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    framebuffer_texture_id_ = setupTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width_, render_height_, 0, GL_RGB, GL_UNSIGNED_BYTE, raytracer_.getFramebuffer().data());
    ImGui::StyleColorsDark();
}

void Gui::renderGui() {
    glfwGetWindowSize(window_, &window_width_, &window_height_);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    rightSideBarResize();

    displayMainMenu();
    displayRenderedImage();
    displayRightSideBar();
    if(display_imgui_metrics_) ImGui::ShowMetricsWindow();
    if(display_imgui_demo_) ImGui::ShowDemoWindow();
    if(display_imgui_about_) ImGui::ShowAboutWindow();
    if(display_imgui_userguide_) ImGui::ShowUserGuide();
    if(display_save_as_) displaySaveAs();

    ImGui::Render();
}

void Gui::renderDrawData() const {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}