#ifndef RAYTRACER_WIDGETS_H
#define RAYTRACER_WIDGETS_H

#include "GLFW/glfw3.h"
#include "imgui.h"
#include <vector>
#include <memory>
#include "Graphics.h"

// Contains a set of widgets that can be drawn using Widget::draw().
// Generally widgets are drawn as separate ImGui windows but exceptions exist such as
// drawing the MainMenuBar or Dockspace.

namespace Editor
{
    class Widget
    {
    public:
        Widget(const char *name);
        inline void  open()  { m_open = true; }
        inline void  close() { m_open = false; }
        virtual void draw() = 0;

    protected:
        bool        m_open = true;
        const char *m_name;
    };

    class TextureViewer : public Widget
    {
    public:
        TextureViewer(const char* name);
        TextureViewer(const char* name, TextureStore* textureStore);
        void draw() override;
        void setTextureStore(TextureStore* textureStore) { m_TEXTURE_STORE = textureStore; }

    private:
        TextureStore * m_TEXTURE_STORE = nullptr;
    };

    // Just shows default Dear Imgui demo widget.
    // Mainly for testing things out
    class DemoWidget : public Widget
    {
    public:
        DemoWidget();
        void draw() override;
    };

    // Shows the main menubar at the top of the main window
    void drawMainMenuBar();
    // Dockspace simply allows windows to be docked to the main window
    void drawDockspace(const char* name, ImGuiID dockspaceID, const ImGuiIO& io);
    void moveTextureWhenDragged(float& offsetX, float& offsetY);
    void zoomTextureWhenScrolled();
}

typedef std::vector<std::unique_ptr<Editor::Widget>> WidgetStore;

#endif //RAYTRACER_WIDGETS_H