#include <GL/glew.h>
#include "Gui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <random>
#include <primitives/Sphere.h>
#include <string>
#include "primitives/Bvhnode.h"
#include "primitives/Sphere.h"
#include "materials/Mix.h"

Gui::Gui(GLFWwindow *window) :

        window(window),
        render_width(100),
        render_height(100),
        render_samples(10),

        display_imgui_metrics(false),
        display_imgui_demo(false),
        display_imgui_about(false),
        display_imgui_userguide(false),
        display_save_as(false),
        display_menu_debug(true),
        display_menu_file(true),
        display_menu_window(true),

        texture_offset(ImVec2(0, 0)),

        camera(90,
               static_cast<float>(render_width/render_height),
               Vector3D(0,0,0),
               Vector3D(1,0,0),
               Vector3D(0,1,0)),
        raytracer(nullptr, camera, render_width, render_height),

        framebuffer_texture_id(0),
        framebuffer_texture_uv0(ImVec2(0, 0)),
        framebuffer_texture_uv1(ImVec2(1, 1)) {

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
        for(const menuitem &item : mainmenu) {
            if(item.display && ImGui::BeginMenu(item.label.c_str())) {
                for (const menuitem &subitem : *item.submenu) {
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

    // set wrap parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}

void Gui::init() {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    framebuffer_texture_id = setupTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width, render_height, 0, GL_RGB, GL_UNSIGNED_BYTE, raytracer.getFramebuffer().data());
    ImGui::StyleColorsDark();
}

void Gui::renderGui() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    displayMainMenu();
    displayRenderedImage();
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

    int screen_width = 0;
    int screen_height = 0;
    glfwGetWindowSize(window, &screen_width, &screen_height);

    ImGui::SetNextWindowPos(ImVec2(0,19));
    ImGui::SetNextWindowSize(ImVec2(screen_width, screen_height));
    ImGui::Begin("Rendered image", nullptr, static_window_flags);
    ImVec2 window_size = ImGui::GetWindowSize();

    if(ImGui::IsWindowFocused()) {
        moveTextureWhenDragged();
        zoomTextureWhenScrolled();
    }

    ImVec2 texture_center = ImVec2((window_size.x - texture_width) * 0.5f + texture_offset.x,
                                   (window_size.y - texture_height) * 0.5f + texture_offset.y);

    ImGui::SetCursorPos(texture_center);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, render_width, render_height, GL_RGB, GL_UNSIGNED_BYTE, raytracer.getFramebuffer().data());
    ImGui::Image((void*)(intptr_t)framebuffer_texture_id, ImVec2(texture_width, texture_height));
    ImGui::BeginGroup();

    if(ImGui::Button("Render")) {
        startRaytracer();
    }

    ImGui::SameLine();

    if(ImGui::Button("Clear")) {
        raytracer.clearFramebuffer();
    }

    ImGui::EndGroup();

    std::string pt = std::to_string(framebuffer_texture_uv0.x) + " " + std::to_string(framebuffer_texture_uv0.y);
    std::string p2 = std::to_string(framebuffer_texture_uv1.x) + " " + std::to_string(framebuffer_texture_uv1.y);
    ImGui::Text(pt.c_str());
    ImGui::Text(p2.c_str());


    ImGui::End();
}

void Gui::startRaytracer() {
    auto invokeRaytraceRender = [](Raytracer &raytracer, int samples) {
        raytracer.render(samples);
    };
    auto th = std::thread(invokeRaytraceRender, std::ref(raytracer), render_samples);
    th.detach();
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
    if(zoom != 0) {
        if(texture_height * zoom > 0 &&
           texture_width * zoom > 0) {
            texture_height *= zoom;
            texture_width *= zoom;
        }
    }
}
