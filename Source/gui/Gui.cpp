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

inline void toggle(bool &value) {
    value ? value = false : value = true;
}

struct menuitem {
    const char *label;
    bool &display;
    menuitem *submenu = nullptr;
    const unsigned int submenu_size;
};

Gui::Gui(GLFWwindow *window) :
        window(window),
        render_width(800),
        render_height(600),
        render_samples(1),
        display_imgui_metrics(false),
        display_imgui_demo(false),
        display_imgui_about(false),
        display_imgui_userguide(false),
        display_save_as(false),
        display_menu_debug(true),
        display_menu_file(true),
        display_menu_window(true),
        display_rendered_image(true),
        world(std::make_unique<std::vector<Primitive>>()),
        camera(90, // default camera
               static_cast<float>(16/9),
               Vector3D(0,0,0),
               Vector3D(1,0,0),
               Vector3D(0,1,0)),
        raytracer(std::make_shared<Raytracer>(nullptr, camera, render_width, render_height)),
        renderthread(nullptr),
        ogl_texture_id(0) {
}

Gui::~Gui() {
    ImGui::DestroyContext();
}

void Gui::displayMainMenu() {

    const unsigned int file_submenu_size = 1;
    menuitem file_submenu[file_submenu_size] = {
            {"Save as", display_save_as}
    };

    const unsigned int debug_submenu_size = 4;
    menuitem debug_submenu[debug_submenu_size] = {
            {"ImGui metrics",    display_imgui_metrics},
            {"ImGui demo",       display_imgui_demo},
            {"ImGui about",      display_imgui_about},
            {"ImGui user guide", display_imgui_userguide},
    };

    const unsigned int window_submenu_size = 1;
    menuitem window_submenu[window_submenu_size] = {
            {"Rendered image", display_rendered_image}
    };

    menuitem mainmenu[] = {
            {"File",   display_menu_file,   file_submenu,   file_submenu_size},
            {"Window", display_menu_window, window_submenu, window_submenu_size},
            {"Debug",  display_menu_debug,  debug_submenu,  debug_submenu_size}
    };

    if(ImGui::BeginMainMenuBar()) {
        for(menuitem item : mainmenu) {
            if(item.display && ImGui::BeginMenu(item.label)) {
                if(item.submenu) {
                    for (int i = 0; i < item.submenu_size; ++i) {
                        menuitem &subitem = item.submenu[i];
                        if(ImGui::MenuItem(subitem.label, 0, subitem.display)) {
                            toggle(subitem.display);
                        }
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width, render_height, 0, GL_RGB, GL_UNSIGNED_BYTE, raytracer->getFramebuffer());
    ImGui::Begin("Rendered image", &display_rendered_image);
    ImGui::Image((void*)(intptr_t)ogl_texture_id, ImVec2(render_width, render_height));
    ImGui::BeginGroup();
    if(ImGui::Button("Render")) {
        startRaytracer();
    }
    ImGui::SameLine();
    if(ImGui::Button("Clear")) {
        raytracer->setWidth(render_width);
    }
    ImGui::EndGroup();
    ImGui::End();
}

void Gui::startRaytracer() {
    auto invokeRaytracerRender = [](std::shared_ptr<Raytracer> &raytracer, int samples) {
        raytracer->render(samples);
    };
    renderthread = std::make_shared<std::thread>(invokeRaytracerRender, raytracer, render_samples);
    renderthread->detach();
}
