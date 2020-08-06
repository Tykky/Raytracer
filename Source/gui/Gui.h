#ifndef RAYTRACER_GUI_H
#define RAYTRACER_GUI_H

#include <GLFW/glfw3.h>
#include <core/Raytracer.h>
#include <core/Camera.h>
#include <primitives/Primitive.h>
#include <thread>
#include <vector>

class Gui {

private:
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

    int render_width;
    int render_height;
    int preview_width;
    int preview_height;

    int render_samples;
    Camera camera;
    Raytracer raytracer;
    std::thread render_thread;

    GLuint ogl_texture_id;

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
