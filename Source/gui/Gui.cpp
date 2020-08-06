#include <GL/glew.h>
#include "Gui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <random>
#include <primitives/Sphere.h>
#include <imgui_internal.h>
#include "primitives/Bvhnode.h"
#include "primitives/Sphere.h"
#include "materials/Mix.h"

struct menuitem {
    std::string label;
    bool &display;
    const std::vector<menuitem> &submenu = {};
};

Gui::Gui(GLFWwindow *window) :
        window(window),
        render_width(2560),
        render_height(1440),
        preview_width(800),
        preview_height(600),
        render_samples(100),
        display_imgui_metrics(false),
        display_imgui_demo(false),
        display_imgui_about(false),
        display_imgui_userguide(false),
        display_save_as(false),
        display_menu_debug(true),
        display_menu_file(true),
        display_menu_window(true),
        display_rendered_image(true),
        camera(90,
               static_cast<float>(16/9),
               Vector3D(0,0,0),
               Vector3D(1,0,0),
               Vector3D(0,1,0)),
        raytracer(nullptr, camera, render_width, render_height),
        ogl_texture_id(0) {
}

Gui::~Gui() {
    ImGui::DestroyContext();
}

void Gui::displayMainMenu() {

    const std::vector<menuitem> file_submenu = {
            {"Save as", display_save_as}
    };

    const std::vector<menuitem> debug_submenu = {
            {"ImGui metrics",    display_imgui_metrics},
            {"ImGui demo",       display_imgui_demo},
            {"ImGui about",      display_imgui_about},
            {"ImGui user guide", display_imgui_userguide},
    };

    const std::vector<menuitem> window_submenu = {
            {"Rendered image", display_rendered_image}
    };

    const std::vector<menuitem> mainmenu = {
            {"File",   display_menu_file,   file_submenu},
            {"Window", display_menu_window, window_submenu},
            {"Debug",  display_menu_debug,  debug_submenu}
    };

    if(ImGui::BeginMainMenuBar()) {
        for(const menuitem &item : mainmenu) {
            if(item.display && ImGui::BeginMenu(item.label.c_str())) {
                for (menuitem subitem : item.submenu) {
                    if(ImGui::MenuItem(subitem.label.c_str(), nullptr, subitem.display)) {
                        subitem.display ^= true;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}

void Gui::init() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ogl_texture_id = setupTexture();
}

void Gui::renderGui() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    displayMainMenu();
    if(display_imgui_metrics) ImGui::ShowMetricsWindow();
    if(display_imgui_demo) ImGui::ShowDemoWindow();
    if(display_imgui_about) ImGui::ShowAboutWindow();
    if(display_imgui_userguide) ImGui::ShowUserGuide();
    if(display_save_as) displaySaveAs();
    if(display_rendered_image) displayRenderedImage();

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width, render_height, 0, GL_RGB, GL_UNSIGNED_BYTE, raytracer.getFramebuffer().data());
    ImGui::Begin("Rendered image", &display_rendered_image);
    ImGui::Image((void*)(intptr_t)ogl_texture_id, ImVec2(800, 600));
    ImGui::BeginGroup();

    if(ImGui::Button("Render")) {
        startRaytracer();
    }

    ImGui::SameLine();

    if(ImGui::Button("Clear")) {
        raytracer.clearFramebuffer();
    }

    ImGui::EndGroup();
    ImGui::End();
}

void Gui::startRaytracer() {
    auto invokeRaytraceRender = [](Raytracer &raytracer, int samples) {
        raytracer.render(samples);
    };
    render_thread = std::thread(invokeRaytraceRender, std::ref(raytracer), render_samples);
    render_thread.detach();
}