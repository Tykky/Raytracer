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

        window(window),
        render_width(2560),
        render_height(1440),
        render_samples(10),

        display_imgui_metrics(false),
        display_imgui_demo(false),
        display_imgui_about(false),
        display_imgui_userguide(false),
        display_save_as(false),
        display_menu_debug(true),
        display_menu_file(true),
        display_menu_window(true),

        main_menubar_height(19),

        texture_offset(ImVec2(0, 0)),
        right_side_bar_width(300),
        right_side_bar_min_width(10),
        right_side_bar_max_width(300),
        right_side_bar_hover_margin(10),

        is_right_side_bar_resizing(false),

        static_window_flags(ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoTitleBar |
                            ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoBringToFrontOnFocus |
                            ImGuiWindowFlags_NoScrollWithMouse |
                            ImGuiWindowFlags_NoScrollbar),

        camera(90,
               static_cast<float>(render_width)/static_cast<float>(render_height),
               Vector3D(0,0,0),
               Vector3D(1,0,0),
               Vector3D(0,1,0)),

        raytracer(nullptr, camera, render_width, render_height),

        framebuffer_texture_id(0) {

    file_submenu = {
            {"Save as", &display_save_as}
    };

    debug_submenu = {
            {"ImGui metrics",    &display_imgui_metrics},
            {"ImGui demo",       &display_imgui_demo},
            {"ImGui about",      &display_imgui_about},
            {"ImGui user guide", &display_imgui_userguide},
    };

    window_submenu = {
    };

    mainmenu = {
            {"File",   &display_menu_file,   &file_submenu},
            {"Window", &display_menu_window, &window_submenu},
            {"Debug",  &display_menu_debug,  &debug_submenu}
    };

    texture_width = static_cast<float>(render_width);
    texture_height = static_cast<float>(render_height);

}

Gui::~Gui() {
    ImGui::DestroyContext();
}

void Gui::displayMainMenu() {
    if(ImGui::BeginMainMenuBar()) {
        for(const Menuitem &item : mainmenu) {
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
    ImGui::Begin("Save as", &display_save_as);
    ImGui::Text("Save rendered image to .ppm file");
    const unsigned int size = 512;
    char filename[size] = {};
    ImGui::InputText("filename", filename, size);
    ImGui::Button("Save");
    ImGui::End();
}

void Gui::displayRenderedImage() {
    ImGui::SetNextWindowPos(ImVec2(0, main_menubar_height));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width) - right_side_bar_width, static_cast<float>(window_height)));
    ImGui::Begin("Rendered image", nullptr, static_window_flags);
    ImVec2 window_size = ImGui::GetWindowSize();

    if(ImGui::IsWindowFocused()) {
        moveTextureWhenDragged();
        zoomTextureWhenScrolled();
    }

    ImVec2 texture_center = ImVec2((window_size.x - texture_width) * 0.5f + texture_offset.x,
                                   (window_size.y - texture_height) * 0.5f + texture_offset.y);

    ImGui::SetCursorPos(texture_center);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, render_width, render_height,
                    GL_RGB, GL_UNSIGNED_BYTE, raytracer.getFramebuffer().data());

    ImGui::Image((void*)(intptr_t)framebuffer_texture_id, ImVec2(texture_width, texture_height));
    ImGui::End();
}

void Gui::displayRightSideBar() {
    ImGui::SetNextWindowSize(ImVec2(right_side_bar_width,static_cast<float>(window_height) - main_menubar_height));
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(window_width) - right_side_bar_width,main_menubar_height));
    ImGui::Begin("right_side_bar",nullptr, static_window_flags);
    ImGui::BeginChild("render_settings",ImVec2(right_side_bar_width,120), true);
    ImGui::Text("Render settings");
    int *res[] = {&render_width, &render_height};

    if(ImGui::InputInt2("Resolution", *res)) {
        raytracer.setWidth(render_width);
        raytracer.setHeight(render_height);
        raytracer.getFramebuffer();
    }

    ImGui::InputInt("Samples", &render_samples);
    if(ImGui::Button("Render")) {
        startRaytracer();
    }

    ImGui::SameLine(65);
    if(ImGui::Button("Clear")) {
        raytracer.clearFramebuffer();
    }

    ImGui::EndChild();
    ImGui::End();
}

void Gui::startRaytracer() {
    auto invokeRaytraceRender = [](Raytracer &raytracer, int samples) {
        raytracer.render(samples);
    };
    auto th = std::thread(invokeRaytraceRender, std::ref(raytracer), render_samples);
    th.detach();
}

void Gui::rightSideBarResize() {
    ImVec2 hover_min(static_cast<float>(window_width) - right_side_bar_width, 0);
    ImVec2 hover_max(static_cast<float>(window_width) - right_side_bar_width + right_side_bar_hover_margin, static_cast<float>(window_height));
    if(ImGui::IsMouseHoveringRect(hover_min, hover_max, false)) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        if(ImGui::IsMouseClicked(0)) {
            is_right_side_bar_resizing = true;
        }
    }
    if(is_right_side_bar_resizing) {
        if(ImGui::IsMouseReleased(0)) {
            is_right_side_bar_resizing = false;
        }
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        float amount = ImGui::GetMouseDragDelta(0).x * (-1);
        ImGui::ResetMouseDragDelta();
        if(right_side_bar_width + amount > right_side_bar_max_width) {
            right_side_bar_width = right_side_bar_max_width;
        } else if(right_side_bar_width + amount < right_side_bar_min_width) {
            right_side_bar_width = right_side_bar_min_width;
        } else {
            right_side_bar_width += amount;
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
        texture_offset.x += mouse_delta.x;
        texture_offset.y += mouse_delta.y;
    }
}

void Gui::zoomTextureWhenScrolled() {
    float zoom = 1 + ImGui::GetIO().MouseWheel/10;
    if(zoom != 0 && texture_height * zoom > 0 && texture_width * zoom > 0) {
        texture_width *= zoom;
        texture_height *= zoom;
    }
}

void Gui::init() {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    framebuffer_texture_id = setupTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width, render_height, 0,GL_RGB, GL_UNSIGNED_BYTE, raytracer.getFramebuffer().data());
    ImGui::StyleColorsDark();
}

void Gui::renderGui() {
    glfwGetWindowSize(window, &window_width, &window_height);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    rightSideBarResize();

    displayMainMenu();
    displayRenderedImage();
    displayRightSideBar();
    if(display_imgui_metrics) ImGui::ShowMetricsWindow();
    if(display_imgui_demo) ImGui::ShowDemoWindow();
    if(display_imgui_about) ImGui::ShowAboutWindow();
    if(display_imgui_userguide) ImGui::ShowUserGuide();
    if(display_save_as) displaySaveAs();

    ImGui::Render();
}

void Gui::renderDrawData() const {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}