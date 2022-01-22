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
        Widget(const char* name, unsigned int id);
        inline void  open()  { m_open = true; }
        inline void  close() { m_open = false; }
        virtual void draw() = 0;

    protected:
        bool         m_open = true;
        std::string  m_name;
        unsigned int m_id;
    };

    class TextureViewer : public Widget
    {
    public:
        TextureViewer(const char* name, unsigned int id, TextureStore* textureStore);
        void draw() override;

    private:
        TextureStore * m_TEXTURE_STORE = nullptr;
    };

    // TODO: Sometimes docking the LogViewer causes mismatching begin()/end() calls
    class LogViewer : public Widget
    {
    public:
        LogViewer(unsigned int id);
        void draw() override;
    };

    // Shows the main menubar at the top of the main window
    void drawMainMenuBar();
    // Dockspace simply allows windows to be docked to the main window
    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io);
    void moveTextureWhenDragged(float& offsetX, float& offsetY);
    void zoomTextureWhenScrolled(float& width, float& height);
    void pushMessagetypeImGuiStyleVar(MessageType type);
}

typedef std::vector<std::unique_ptr<Editor::Widget>> WidgetStore;

#endif //RAYTRACER_WIDGETS_H