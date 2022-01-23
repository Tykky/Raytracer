#ifndef RAYTRACER_WIDGETS_H
#define RAYTRACER_WIDGETS_H

#include "GLFW/glfw3.h"
#include "imgui.h"
#include <vector>
#include <memory>
#include <string>
#include "Graphics.h"
#include "logging/Logging.h"

// Contains a set of widgets that can be drawn using Widget::draw().
// Generally widgets are drawn as separate ImGui windows but exceptions exist such as
// drawing the MainMenuBar or Dockspace.

namespace Editor
{
    class Widget
    {
    public:
        Widget(const char* name);
        inline void  open()  { m_open = true; }
        inline void  close() { m_open = false; }
        inline void setId(unsigned int id) { m_id = id; }
        virtual void draw() = 0;
        inline const std::string& getName() const { return m_name; }

    protected:
        bool          m_open = true;
        std::string   m_name;
        unsigned int  m_id = 0;
    };

    class TextureViewer : public Widget
    {
    public:
        TextureViewer(TextureStore* textureStore);
        void draw() override;

    private:
        TextureStore* m_textStore = nullptr;
    };

    class LogViewer : public Widget
    {
    public:
        LogViewer();
        void draw() override;
    };

    class WidgetStore
    {
    public:
        inline unsigned int size() const { return m_widgets.size(); }

        inline Widget* operator[](int idx) const
        {
            assert(idx < m_widgets.size());
            return m_widgets[idx].get();
        }

        inline void push(std::unique_ptr<Widget>&& widget)
        {
            widget->setId(m_currentUniqueIdx++);
            m_widgets.push_back(std::move(widget));
        }

    private:
        std::vector<std::unique_ptr<Widget>> m_widgets;
        unsigned int m_currentUniqueIdx = 0; // Used to create unique id for new widget
    };

    class WidgetInspector : public Widget
    {
    public:
        WidgetInspector(WidgetStore* widgetStore);
        void draw() override;
    private:
        WidgetStore* m_widgetStore;
    };

    // Shows the main menubar at the top of the main window
    void drawMainMenuBar();
    // Dockspace simply allows windows to be docked to the main window
    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io);
    void moveTextureWhenDragged(float& offsetX, float& offsetY);
    void zoomTextureWhenScrolled(float& width, float& height);
    void pushMessagetypeImGuiStyleVar(MessageType type);
}

#endif //RAYTRACER_WIDGETS_H