#ifndef RAYTRACER_GUI_H
#define RAYTRACER_GUI_H

#include <GLFW/glfw3.h>
#include <vector>

class Gui {

private:
    GLFWwindow  *window;
    bool display_imgui_metrics;
    bool display_imgui_demo;
    bool display_imgui_about;
    bool display_imgui_userguide;
    bool display_save_as;
    bool display_menu_file;
    bool display_menu_debug;
    bool display_menu_help;

    unsigned int setupTexture() const;
    void mainMenu();
    void saveAs();

public:
    Gui(GLFWwindow *window);
    ~Gui();
    /** @brief initializes imgui context & opengl. Call this
     * before you call renderGui(). */
    void init() const;
    void renderGui();
    /** @brief Draws the rendered data to screen. call this
     * after renderGui(). */
    void renderDrawData() const;
};


#endif //RAYTRACER_GUI_H
