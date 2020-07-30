#include <GL/glew.h>
#include "Gui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

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
        display_imgui_metrics(false),
        display_imgui_demo(false),
        display_imgui_about(false),
        display_imgui_userguide(false),
        display_save_as(false),
        display_menu_debug(true),
        display_menu_file(true) {
}

Gui::~Gui() {
    ImGui::DestroyContext();
}

void Gui::mainMenu() {

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

    menuitem mainmenu[] = {
            {"File", display_menu_file, file_submenu, file_submenu_size},
            {"Debug", display_menu_debug, debug_submenu, debug_submenu_size}
    };

    if(ImGui::BeginMainMenuBar()) {
        for(menuitem item : mainmenu) {
            if(ImGui::BeginMenu(item.label, item.display)) {
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

void Gui::init() const {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void Gui::renderGui() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    mainMenu();
    if(display_imgui_metrics) ImGui::ShowMetricsWindow();
    if(display_imgui_demo) ImGui::ShowDemoWindow();
    if(display_imgui_about) ImGui::ShowAboutWindow();
    if(display_imgui_userguide) ImGui::ShowUserGuide();
    if(display_save_as) saveAs();

    ImGui::Render();
}

void Gui::renderDrawData() const {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::saveAs() {
    ImGui::Begin("Save as");
    ImGui::Text("Save rendered image to .ppm file");
    const size_t size = 512;
    char filename[size] = {};
    ImGui::InputText("filename", filename, size);
    ImGui::Button("Save");
    std::cout << filename << std::endl;
    ImGui::End();
}
