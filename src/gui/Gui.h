#ifndef RAYTRACER_GUI_H
#define RAYTRACER_GUI_H

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <core/Raytracer.h>
#include <core/Camera.h>
#include <hittables/Hittable.h>
#include <thread>
#include <vector>
#include <random>
#include <deque>
#include <materials/Lambertian.h>
#include <materials/Metal.h>
#include <materials/Dielectric.h>

class Gui {

public:
    /**
     * @brief Handles everything gui related.
     * Initialize GLFW before calling this.
     * @param window
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

    bool display_imgui_metrics_ = false;
    bool display_imgui_demo_ = false;
    bool display_imgui_about_ = false;
    bool display_imgui_userguide_ = false;
    bool display_save_as_ = false;
    bool display_menu_file_ = true;
    bool display_menu_debug_ = false;
    bool display_menu_window_ = true;

    std::vector<Menuitem> file_submenu_ = {
            {"Save as", &display_imgui_metrics_}
    };
    std::vector<Menuitem> debug_submenu_ = {
            {"ImGui metrics",    &display_imgui_metrics_},
            {"ImGui demo",       &display_imgui_demo_},
            {"ImGui about",      &display_imgui_about_},
            {"ImGui user guide", &display_imgui_userguide_},
    };
    std::vector<Menuitem> window_submenu_ = {};
    std::vector<Menuitem> mainmenu_ = {
            {"File",   &display_menu_file_,   &file_submenu_},
            {"Debug",  &display_menu_debug_,  &debug_submenu_}
    };

    float main_menubar_height_ = 19;

    float right_side_bar_width_ = 300;
    const float right_side_bar_min_width_ = 10;
    const float right_side_bar_max_width_ = 300;
    const float right_side_bar_hover_margin_ = 10;
    bool is_right_side_bar_resizing_ = false;
    float samples_per_second_ = 1000;

    float perf_monitor_height_ = 120;
    int perf_monitor_resolution_ = 1000;
    std::vector<float> perf_monitor_graph_data_ = std::vector<float>(perf_monitor_resolution_);

    float texture_width_;
    float texture_height_;
    ImVec2 texture_offset_ = {0,0};

    int render_width_ = 2560;
    int render_height_ = 1440;
    int render_samples_ = 1000;
    Camera camera_ = {};

    float camera_fov_ = 90.f;
    float camera_pos_x_ = 0.f;
    float camera_pos_y_ = 0.f;
    float camera_pos_z_ = 0.f;
    float camera_up_x_ = 0.f;
    float camera_up_y_ = 0.f;
    float camera_up_z_ = 0.f;

    int current_hittable = 0;
    char current_hittable_name[20];
    float current_hittable_pos_x_ = 0.f;
    float current_hittable_pos_y_ = 0.f;
    float current_hittable_pos_z_ = 0.f;
    int current_material_ = 0.f;

    float current_sphere_radius_ = 0.f;

    int randomizer_sphere_count_ = 10000;
    int randomizer_scatter_multiplier_ = 25;

    // Indexes match between hittable_names and world
    std::vector<char *> hittable_names_ = {}; // used only for GUI
    std::vector<char *> material_names = {
            "Lambertian",
            "Metal",
            "Dielectric"
    }; // used only for GUI
    std::vector<std::shared_ptr<Hittable>> world_ = {}; // used for rendering
    std::shared_ptr<Hittable> bvh = {};

    // Randomized materials are inserted here
    std::vector<std::unique_ptr<Material>> world_materials_ = {};

    // Materials used for "Add spheres" section
    std::unique_ptr<Material> lambertian_ = std::make_unique<Lambertian>(Vector3D(0.5f, 0.5f, 0.5f));
    std::unique_ptr<Material> metal_ = std::make_unique<Metal>(Vector3D(0.8f,0.6f,0.4f), 0.3f);
    std::unique_ptr<Material> dielectric_ = std::make_unique<Dielectric>(1.3f);
    std::unique_ptr<Material> mix_;

    // random generator
    std::mt19937 generator = {};
    std::uniform_real_distribution<float> dist = std::uniform_real_distribution<float>(0,1);
    std::function<float()> randomFloat = std::bind(dist,generator);

    Raytracer raytracer_ = {nullptr, &camera_, render_width_, render_height_};

    GLuint framebuffer_texture_id_;

    const unsigned int static_window_flags_ = ImGuiWindowFlags_NoMove |
                                              ImGuiWindowFlags_NoTitleBar |
                                              ImGuiWindowFlags_NoResize |
                                              ImGuiWindowFlags_NoBringToFrontOnFocus;

    unsigned int setupTexture() const;
    void displayMainMenu();
    void displaySaveAs();
    void displayRenderedImage();

    void displayRightSideBar();
    void displayRenderSettingsChild(const ImVec2 &size);
    void displayCameraSettingsChild(const ImVec2 &size);
    void displayObjectsChild(const ImVec2 &size);
    void displayRandomizerChild(const ImVec2 &size);
    void displayPerfMonitor();

    void startRaytracer();
    void rightSideBarResize();
    void resizeWindow(const ImVec2 &hover_min, const ImVec2 &hover_max, float &resize_pos,
                      const float &resize_pos_min, const float &resize_pos_max, bool &is_resizing,
                      const orientation &resize_orientation);

    void moveTextureWhenDragged();
    void zoomTextureWhenScrolled();

    void randomizeWorld(const int &spheres, const int &scatter);
};


#endif //RAYTRACER_GUI_H
