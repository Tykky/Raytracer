#ifndef RAYTRACER_GUI_H
#define RAYTRACER_GUI_H

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <core/Raytracer.h>
#include <core/Camera.h>
#include <hittables/Hittable.h>
#include <thread>
#include <vector>

class Gui {

public:
    /**
     * @brief Handles everything gui related.
     * Initialize GLFW before calling this.
     * @param [in] window
     */
    Gui(GLFWwindow *window);
    ~Gui();

    /** @brief Initializes imgui context & OpenGl. Call this
     * before you call renderGui(). */
    void init();

    /** @Brief Call this in between init() and renderDrawData() */
    void renderGui();

    /** @brief Draws the rendered data to the screen. Call this
     * after renderGui(). */
    void renderDrawData() const;


private:

    struct Menuitem {
        std::string label;
        bool *display = nullptr;
        std::vector<Menuitem> *submenu = nullptr;
    };

    enum class orientation { HORIZONTAL, VERTICAL};

    GLFWwindow *window;

    int window_width;
    int window_height;

    bool display_imgui_metrics;
    bool display_imgui_demo;
    bool display_imgui_about;
    bool display_imgui_userguide;
    bool display_save_as;
    bool display_menu_file;
    bool display_menu_debug;
    bool display_menu_window;

    std::vector<Menuitem> file_submenu;
    std::vector<Menuitem> debug_submenu;
    std::vector<Menuitem> window_submenu;
    std::vector<Menuitem> mainmenu;

    float main_menubar_height;

    float right_side_bar_width;
    const float right_side_bar_min_width;
    const float right_side_bar_max_width;
    const float right_side_bar_hover_margin;
    bool is_right_side_bar_resizing;

    float texture_width;
    float texture_height;
    ImVec2 texture_offset;

    int render_width;
    int render_height;
    int render_samples;
    Camera camera;
    Raytracer raytracer;

    GLuint framebuffer_texture_id;

    const unsigned int static_window_flags;

    unsigned int setupTexture() const;
    void displayMainMenu();
    void displaySaveAs();
    void displayRenderedImage();
    void displayRightSideBar();

    void startRaytracer();
    void rightSideBarResize();
    void resizeWindow(const ImVec2 &hover_min, const ImVec2 &hover_max, float &resize_pos,
                      const float &resize_pos_min, const float &resize_pos_max, bool &is_resizing,
                      const orientation &resize_orientation);

    void moveTextureWhenDragged();
    void zoomTextureWhenScrolled();

};


#endif //RAYTRACER_GUI_H
