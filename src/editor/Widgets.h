#ifndef RAYTRACER_WIDGETS_H
#define RAYTRACER_WIDGETS_H

#include "GLFW/glfw3.h"
#include "imgui.h"
#include <vector>
#include <memory>
#include <string>
#include "Graphics.h"
#include "logging/Logging.h"
#include "ImFileDialog.h"
#include <functional>

// Contains a set of widgets that can be drawn using Widget::draw().
// Generally widgets are drawn as separate ImGui windows but exceptions exist such as
// drawing the MainMenuBar or Dockspace.

namespace Editor
{
    class Widget
    {
    public:
        Widget(const char* name);
        inline bool isOpen() { return m_open; }
        inline void setId(unsigned int id) { m_id = id; }
        inline void setWindowId(std::string&& windowId) { m_windowId = std::move(windowId); }
        inline unsigned int getId() const { return m_id; }
        inline const std::string& getName() const { return m_name; }
        virtual void draw() = 0;

    protected:
        bool                m_open = true;
        const std::string   m_name;     // name displayed
        unsigned int        m_id = 0;   // ID
        // Used to ID Dear Imgui windows
        std::string         m_windowId; // name###ID
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
        Gfx::GLtexture* m_currentTexture = nullptr;
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

    private:
        std::vector<std::unique_ptr<Widget>> m_widgets;
        unsigned int m_currentUniqueIdx = 0; // Used to create unique ids for new widgets
    };

    class WidgetInspector : public Widget
    {
    public:
        WidgetInspector(WidgetStore* widgetStore);
        void draw() override;
    private:
        WidgetStore* m_widgetStore;
    };

    class FileDialog : public Widget
    {
    public:
        FileDialog();
        void draw() override;
    private:
        ifd::FileDialog m_fileDialog;
        std::function<void*(uint8_t* data, int w, int h, char fmt)> m_createTexture;
        std::function<void(void* tex)> m_deleteTexture;
    };

    // Shows the main menubar at the top of the main window
    void drawMainMenuBar(WidgetStore& widgetStore, TextureStore& textureStore);
    // Dockspace simply allows windows to be docked to the main window
    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io);
    void moveTextureWhenDragged(float& offsetX, float& offsetY);
    void zoomTextureWhenScrolled(float& width, float& height);
    void pushMessagetypeImGuiStyleVar(MessageType type);
}

#endif //RAYTRACER_WIDGETS_H