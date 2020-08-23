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

    GLFWwindow *window_;

    int window_width_;
    int window_height_;

    bool display_imgui_metrics_;
    bool display_imgui_demo_;
    bool display_imgui_about_;
    bool display_imgui_userguide_;
    bool display_save_as_;
    bool display_menu_file_;
    bool display_menu_debug_;
    bool display_menu_window_;

    std::vector<Menuitem> file_submenu_;
    std::vector<Menuitem> debug_submenu_;
    std::vector<Menuitem> window_submenu_;
    std::vector<Menuitem> mainmenu_;

    float main_menubar_height_;

    float right_side_bar_width_;
    const float right_side_bar_min_width_;
    const float right_side_bar_max_width_;
    const float right_side_bar_hover_margin_;
    bool is_right_side_bar_resizing_;

    float texture_width_;
    float texture_height_;
    ImVec2 texture_offset_;

    int render_width_;
    int render_height_;
    int render_samples_;
    Camera camera_;
    Raytracer raytracer_;

    GLuint framebuffer_texture_id_;

    const unsigned int static_window_flags_;

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
