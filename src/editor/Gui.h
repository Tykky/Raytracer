#ifndef RAYTRACER_GUI_H
#define RAYTRACER_GUI_H

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <core/Raytracer.h>
#include <core/Camera.h>
#include <primitives/Primitive.h>
#include <thread>
#include <vector>
#include <random>
#include <deque>
#include <materials/Lambertian.h>
#include <materials/Metal.h>
#include <materials/Dielectric.h>

struct Menuitem
{
    std::string label;
    bool* display = nullptr;
    std::vector<Menuitem>* submenu = nullptr;
};

enum class orientation { HORIZONTAL, VERTICAL};

struct GuiState
{
    GLFWwindow* window = nullptr;

    int windowWidth  = 800;
    int windowHeight = 600;

    bool displayImguiMetrics   = false;
    bool displayImguiDemo      = false;
    bool displayImguiAbout     = false;
    bool displayImguiUserGUide = false;
    bool displaySaveAs         = false;
    bool displayMenuFile       = true;
    bool displayMenuDebug      = false;
    bool displayMenuWindow     = true;

    std::vector<Menuitem> fileSubmenu =
    {
        {"Save as", &displaySaveAs}
    };

    std::vector<Menuitem> debugSubmenu =
    {
        {"ImGui metrics",    &displayImguiMetrics},
        {"ImGui demo",       &displayImguiDemo},
        {"ImGui about",      &displayImguiAbout},
        {"ImGui user guide", &displayImguiUserGUide},
    };

    std::vector<Menuitem> windowSubmenu = {};

    std::vector<Menuitem> mainmenu =
    {
        {"File",   &displayMenuFile,   &fileSubmenu},
        {"Debug",  &displayMenuDebug,  &debugSubmenu}
    };

    float mainMenubarHeight = 19;

    float rightSideBarWidth              = 300;
    const float rightSideBarMinWidth     = 10;
    const float rightSideBarMaxWidth     = 300;
    const float rightSideBarHoverMargin  = 10;
    bool isRightSideBarResizing          = false;
    float samplesPerSecond               = 1000;

    float perfMonitorHeight = 120;
    int perfMonitorResolution = 1000;
    std::vector<float> perfMonitorGraphData = std::vector<float>(perfMonitorResolution);

    float textureWidth;
    float textureHeight;
    ImVec2 textureOffset = {0,0};

    int renderWidth = 1280;
    int renderHeight = 720;
    int renderSamples = 10;
    Camera camera = {};

    float cameraFov = 90.f;
    float cameraPosX = 0.f;
    float cameraPosY = 0.f;
    float cameraPosZ = 0.f;
    float cameraUpX = 0.f;
    float cameraUpY = 0.f;
    float cameraUpZ = 0.f;

    int currentHittable = 0;
    char currentHittableName[20] = "";
    float currentHittablePosX = 0.f;
    float currentHittablePosY = 0.f;
    float currentHittablePosZ = 0.f;
    int currentMaterial = 0;
    float currentSphereRadius = 0.f;
    int currrentObjectType = 2;
    float currentVertex0[3] = {0.f, 0.f, 0.f};
    float currentVertex1[3] = {0.f, 0.f, 0.f};
    float currentVertex2[3] = {0.f, 0.f, 0.f};
    float currentNormal[3] = {0.f, 0.f, 0.f};
    char currentObjFilename[100] = "";

    int randomizerSphereCOunt = 10000;
    int randomizerScatterMultiplier = 25;

    // Indexes match between hittable_names and world
    std::vector<char *> hittableNames = {}; // used only for GUI
    std::vector<char *> materialNames;

    std::vector<char *> objectTypes;

    std::vector<std::shared_ptr<Primitive>> world = {}; // used for rendering
    std::shared_ptr<Primitive> bvh = {};

    // Randomized materials are inserted here
    std::vector<std::unique_ptr<Material>> world_materials = {};

    // Materials used for "Add spheres" section
    std::unique_ptr<Material> lambertian = std::make_unique<Lambertian>(Vector3D(0.5f, 0.5f, 0.5f));
    std::unique_ptr<Material> metal = std::make_unique<Metal>(Vector3D(0.8f,0.6f,0.4f), 0.3f);
    std::unique_ptr<Material> dielectric = std::make_unique<Dielectric>(1.3f);
    std::unique_ptr<Material> mix;

    // random generator
    std::mt19937 generator = {};
    std::uniform_real_distribution<float> dist = std::uniform_real_distribution<float>(0,1);
    std::function<float()> randomFloat = std::bind(dist, generator);

    Raytracer raytracer = {nullptr, &camera, renderWidth, renderHeight};

    GLuint framebufferTextureId;

    const unsigned int staticWindowFlags = ImGuiWindowFlags_NoMove       |
                                             ImGuiWindowFlags_NoTitleBar |
                                             ImGuiWindowFlags_NoResize   |
                                             ImGuiWindowFlags_NoBringToFrontOnFocus;
};

// Public api

/**
 * @brief Initializes imgui context & OpenGl. Call this
 * before you call renderGui().
 */
void init(GLFWwindow* window, int& framebufferTextureId, int width, int height, char* framebuffer);

/** @Brief Call this in between init() and renderDrawData() */
void renderGui(GuiState& state);

/**
 * @brief Draws the rendered data to the screen. Call this
 * after renderGui().
 */
void renderDrawData();

namespace // Implementation details
{
    unsigned int setupTexture();
    void displayMainMenu(const std::vector<Menuitem> &mainMenu);
    void displaySaveAs();
    void displayRenderedImage(GuiState &state);
    void displayRightSideBar(const GuiState &state);
    void displayRenderSettingsChild(const ImVec2 &size, GuiState &state);
    void displayCameraSettingsChild(const ImVec2 &size, GuiState &state);
    void displayObjectsChild(const ImVec2 &size, GuiState &state);
    void displayRandomizerChild(const ImVec2 &size, GuiState &state);
    void displayPerfMonitor();
    void startRaytracer(const GuiState &state);
    void rightSideBarResize(GuiState &state);
    void resizeWindow(const ImVec2 &hover_min, const ImVec2 &hover_max, float &resize_pos,
                      const float &resize_pos_min, const float &resize_pos_max, bool &is_resizing,
                      const orientation &resize_orientation);
    void moveTextureWhenDragged(GuiState &state);
    void zoomTextureWhenScrolled(GuiState &state);
    void randomizeWorld(const int &spheres, const int &scatter, GuiState &state);
    void clearObjects(GuiState &state);
}

#endif //RAYTRACER_GUI_H
