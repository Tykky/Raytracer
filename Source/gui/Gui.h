#ifndef RAYTRACER_GUI_H
#define RAYTRACER_GUI_H

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <core/Raytracer.h>
#include <core/Camera.h>
#include <primitives/Primitive.h>
#include <thread>
#include <vector>

class Gui {

private:

    struct menuitem {
        std::string label;
        bool *display = nullptr;
        std::vector<menuitem> *submenu = nullptr;
    };

    GLFWwindow *window;

    bool display_imgui_metrics;
    bool display_imgui_demo;
    bool display_imgui_about;
    bool display_imgui_userguide;
    bool display_save_as;
    bool display_menu_file;
    bool display_menu_debug;
    bool display_menu_window;
    bool display_rendered_image;

    std::vector<menuitem> file_submenu;
    std::vector<menuitem> debug_submenu;
    std::vector<menuitem> window_submenu;
    std::vector<menuitem> mainmenu;

    int render_width;
    int render_height;

    int render_samples;
    Camera camera;
    Raytracer raytracer;
    std::thread render_thread;

    GLuint framebuffer_texture_id;

    // Upper left corner of texture
    ImVec2 framebuffer_texture_uv0;

    // Bottom right corner of the texture
    ImVec2 framebuffer_texture_uv1;

    unsigned int setupTexture() const;
    void displayMainMenu();
    void displaySaveAs();
    void displayRenderedImage();
    void startRaytracer();

public:
    Gui(GLFWwindow *window);
    ~Gui();
    /** @brief Initializes imgui context & OpenGl. Call this
     * before you call renderGui(). */
    void init();
    /** @Brief Call this in between init() and renderDrawData() */
    void renderGui();
    /** @brief Draws the rendered data to screen. Call this
     * after renderGui(). */
    void renderDrawData() const;
};


#endif //RAYTRACER_GUI_H
