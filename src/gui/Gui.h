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

class Gui 
{
public:

    Gui(GLFWwindow *window);
    ~Gui();

    void init();
    void renderGui();
    void renderDrawData() const;

private:

    struct Menuitem 
    {
        std::string label;
        bool* display = nullptr;
        std::vector<Menuitem>* submenu = nullptr;
    };

    enum class orientation { HORIZONTAL, VERTICAL};

    GLFWwindow* m_window;

    int m_windowWidth;
    int m_windowHeight;

    bool m_displayImguiMetrics   = false;
    bool m_displayImguiDemo      = false;
    bool m_displayImguiAbout     = false;
    bool m_displayImguiUserGUide = false;
    bool m_displaySaveAs         = false;
    bool m_displayMenuFile       = true;
    bool m_displayMenuDebug      = false;
    bool m_displayMenuWindow     = true;

    std::vector<Menuitem> m_fileSubmenu = 
    {
        {"Save as", &m_displaySaveAs}
    };

    std::vector<Menuitem> m_debugSubmenu = 
    {
        {"ImGui metrics",    &m_displayImguiMetrics},
        {"ImGui demo",       &m_displayImguiDemo},
        {"ImGui about",      &m_displayImguiAbout},
        {"ImGui user guide", &m_displayImguiUserGUide},
    };

    std::vector<Menuitem> m_windowSubmenu = {};

    std::vector<Menuitem> m_mainmenu = 
    {
        {"File",   &m_displayMenuFile,   &m_fileSubmenu},
        {"Debug",  &m_displayMenuDebug,  &m_debugSubmenu}
    };

    float m_mainMenubarHeight = 19;

    float m_rightSideBarWidth              = 300;
    const float m_rightSideBarMinWidth     = 10;
    const float m_RightSideBarMaxWidth     = 300;
    const float m_rightSideBarHoverMargin  = 10;
    bool m_isRightSideBarResizing          = false;
    float m_SamplesPerSecond               = 1000;

    float m_perfMonitorHeight = 120;
    int m_PerfMonitorResolution = 1000;
    std::vector<float> m_perfMonitorGraphData = std::vector<float>(m_PerfMonitorResolution);

    float m_textureWidth;
    float m_textureHeight;
    ImVec2 m_textureOffset = {0,0};

    int m_renderWidth = 1280;
    int m_renderHeight = 720;
    int m_renderSamples = 10;
    Camera m_camera = {};

    float m_cameraFov = 90.f;
    float m_cameraPosX = 0.f;
    float m_cameraPosY = 0.f;
    float m_cameraPosZ = 0.f;
    float m_cameraUpX = 0.f;
    float m_cameraUpY = 0.f;
    float m_cameraUpZ = 0.f;

    int m_currentHittable = 0;
    char m_currentHittableName[20] = "";
    float m_currentHittablePosX = 0.f;
    float m_currentHittablePosY = 0.f;
    float m_currentHittablePosZ = 0.f;
    int m_currentMaterial = 0;
    float m_currentSphereRadius = 0.f;
    int m_currrentObjectType = 2;
    float m_currentVertex0[3] = {0.f, 0.f, 0.f};
    float m_currentVertex1[3] = {0.f, 0.f, 0.f};
    float m_currentVertex2[3] = {0.f, 0.f, 0.f};
    float m_currentNormal[3] = {0.f, 0.f, 0.f};
    char m_currentObjFilename[100] = "";

    int m_randomizerSphereCOunt = 10000;
    int m_randomizerScatterMultiplier = 25;

    // Indexes match between hittable_names and world
    std::vector<char *> m_hittableNames = {}; // used only for GUI
    std::vector<char *> m_materialNames;

    std::vector<char *> m_objectTypes;

    std::vector<std::shared_ptr<Hittable>> m_world = {}; // used for rendering
    std::shared_ptr<Hittable> m_bvh = {};

    // Randomized materials are inserted here
    std::vector<std::unique_ptr<Material>> m_world_materials = {};

    // Materials used for "Add spheres" section
    std::unique_ptr<Material> m_lambertian = std::make_unique<Lambertian>(Vector3D(0.5f, 0.5f, 0.5f));
    std::unique_ptr<Material> m_metal = std::make_unique<Metal>(Vector3D(0.8f,0.6f,0.4f), 0.3f);
    std::unique_ptr<Material> m_dielectric = std::make_unique<Dielectric>(1.3f);
    std::unique_ptr<Material> m_mix;

    // random generator
    std::mt19937 m_generator = {};
    std::uniform_real_distribution<float> m_dist = std::uniform_real_distribution<float>(0,1);
    std::function<float()> randomFloat = std::bind(m_dist,m_generator);

    Raytracer m_raytracer = {nullptr, &m_camera, m_renderWidth, m_renderHeight};

    GLuint m_framebufferTextureId;

    const unsigned int m_staticWindowFlags = ImGuiWindowFlags_NoMove |
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
    void clearObjects();

};


#endif //RAYTRACER_GUI_H
