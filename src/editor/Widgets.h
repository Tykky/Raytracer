#ifndef RAYTRACER_WIDGETS_H
#define RAYTRACER_WIDGETS_H

#include "GLFW/glfw3.h"
#include "imgui.h"
#include <vector>
#include "Graphics.h"

// Contains a set of widgets that can be drawn using draw().
// Generally widgets are drawn as separate ImGui windows but exceptions exist such as
// drawing the MainMenuBar or Dockspace.

namespace Editor
{
    class Widget
    {
    public:
        Widget(const char* name);
        void close();
        void open();
        virtual void draw() = 0;

    protected:
        bool m_open = true;
        const char* m_name;
    };

    class RenderView : public Widget
    {
    public:
        RenderView(const char* name);
        RenderView(const char* name, GLtexture texture);
        virtual void draw() override;

    private:
        GLtexture m_gltexture; // RenderView draws this
    };

    // Shows the main menubar at the top of the main window
    void drawMainMenuBar();
    // Dockspace simply allows windows to be docked to the main window
    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io);
    void moveTextureWhenDragged(float& offsetX, float& offsetY);
    void zoomTextureWhenScrolled();
}

#endif //RAYTRACER_WIDGETS_H
