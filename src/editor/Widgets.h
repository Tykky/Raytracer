#ifndef RAYTRACER_WIDGETS_H
#define RAYTRACER_WIDGETS_H

#include <GLFW/glfw3.h>
#include "imgui.h"
#include <vector>
#include <memory>
#include <string>
#include "Graphics.h"
#include "logging/Logging.h"
#include "ImFileDialog.h"
#include <functional>
#include "core/Raytracer.h"

// Contains a set of widgets that can be drawn using Widget::draw().
// Generally widgets are drawn as separate Dear ImGui windows but exceptions exist such as
// drawing the MainMenuBar or Dockspace.

namespace Editor
{
    // wrapper around glfw input functions. The key codes directly map to GLFW
    int getKey(int key);
    int getMouseButton(int button);
    float getMouseScroll();

    void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    GLFWwindow* getEditorWindowHandle();

    class Widget
    {
    public:
        Widget(const char* name);
        inline  bool isOpen() { return m_open; }
        inline  void setId(unsigned int id) { m_id = id; }
        inline  void setWindowId(std::string&& windowId) { m_windowId = std::move(windowId); }
        inline  unsigned int getId() const { return m_id; }
        inline  const std::string& getName() const { return m_name; }
        virtual void draw() = 0;

    protected:
        bool                m_open = true;
        const std::string   m_name;     // name displayed
        // Note that ID needs to be explicitly set or otherwise it will be 0
        unsigned int        m_id = 0;   // ID
        // Used to ID Dear Imgui windows
        std::string         m_windowId; // contains string: name###ID
    };

    class TextureViewer : public Widget
    {
    public:
        TextureViewer(TextureStore* textureStore);
        void draw() override;

    private:
        TextureStore*   m_textStore      = nullptr;
        ImVec2          m_offset         = {0.0f, 0.0f};
        ImVec2          m_scale          = {600.0f, 600.0f};
        Texture*        m_currentTexture = nullptr;
    };

    class LogViewer : public Widget
    {
    public:
        LogViewer();
        void draw() override;
    private:
        bool m_srollToBottom = true;
    };

    class WidgetStore
    {
    public:
        unsigned int size() const;
        Widget* operator[](int idx) const;
        void push(std::unique_ptr<Widget>&& widget);
        void erase(int idx);
        std::vector<std::unique_ptr<Widget>>::iterator begin();
        std::vector<std::unique_ptr<Widget>>::iterator end();
        std::vector<std::unique_ptr<Widget>>::const_iterator begin() const;
        std::vector<std::unique_ptr<Widget>>::const_iterator end() const;

    private:
        std::vector<std::unique_ptr<Widget>> m_widgets;
        unsigned int m_currentUniqueIdx = 0; // Used to create unique ids for new widgets
    };

#ifndef NDEBUG
    class WidgetInspector : public Widget
    {
    public:
        WidgetInspector(WidgetStore* widgetStore);
        void draw() override;
    private:
        WidgetStore* m_widgetStore;
    };
#endif

    class SettingsWidget : public Widget
    {
    public:
        SettingsWidget();
        void draw() override;
    };

#ifndef NDEBUG
    // Small wrapper for Dear Imgui metrics command.
    // Used only for debugging
    struct ImGuiMtericsWidget : public Widget
    {
        inline ImGuiMtericsWidget() : Widget("Dear Imgui Metrics") {}
        inline void draw() override { ImGui::ShowMetricsWindow(&m_open); }
    };

    // Small wrapper for Dear ImGui stack tool
    // Used only for debugging
    struct ImguiStackToolWidget : public Widget
    {
        inline ImguiStackToolWidget() : Widget("Dear Imgui Stack tool") {}
        inline void draw() override { ImGui::ShowStackToolWindow(); }
    };
#endif

    class Viewport : public Widget
    {
    public:
        Viewport();
        void draw() override;

        void setViewportTexture(const RenderTexture& renderTexture);
        void setViewportTexture(unsigned int texID);
        void setViewportTextureToColorAttachment(unsigned int);

        inline bool isPrimary() const { return m_isPrimary; }

    private:
        void processInput();

        bool          m_isPrimary = false;
        bool          m_wireframe = false;
        bool          m_firstInputRecieved = true;
        unsigned int  m_resX      = 1920;
        unsigned int  m_resY      = 1080;
        ImVec2        m_offset    = {0.0f, 0.0f};
        ImVec2        m_scale     = {m_resX, m_resY};
        Camera        m_camera    = {static_cast<float>(m_resX)/static_cast<float>(m_resY),  // aspect ratio
                                     {0, 0.0, 3}, // pos
                                     {0.0, 0.0, -1}}; // target
        Framebuffer   m_framebuffer;
        Vertexbuffer  m_vertexbuffer;
        ShaderProgram m_shaderProgram;

        // Viewport shows this texture
        void* m_viewportTexture = nullptr;
    };

    class RTControls : public Widget
    {
    public:
        RTControls(Raytracer* raytracer, WidgetStore* widgetStore, TextureStore* textureStore);
        void draw() override;
    private:
        Raytracer*    m_raytracer;
        int           m_samples = 100;
        WidgetStore*  m_widgetStore;
        TextureStore* m_textureStore;
        unsigned int  m_viewportTexture = 0;
    };

    class SystemInfo : public Widget
    {
    public:
        SystemInfo();
        void draw() override;
    private:
        std::string m_GPUVendor;
        std::string m_renderer;
        std::string m_GLVersion;
        std::string m_GLSLVersion;
    };

    // Shows the main menubar at the top of the main window
    void drawMainMenuBar(WidgetStore& widgetStore, TextureStore& textureStore);
    // Dockspace simply allows windows to be docked to the main window
    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io);
    void drawImFileDialogAndProcessInput();
    void drawTextureView(void* currentTexId, ImVec2& offset, ImVec2& scale, bool& open);
    void drawTexturePickerComboBox(const char* preview, TextureStore* textureStore, Texture*& currentTexture);
    void moveTextureWhenDragged(float& offsetX, float& offsetY);
    void zoomTextureWhenScrolled(float& width, float& height);
    void cleanInactiveWidgets(WidgetStore& widgetStore);
    Viewport* findPrimaryViewport(const WidgetStore& widgetStore);
}
#endif //RAYTRACER_WIDGETS_H