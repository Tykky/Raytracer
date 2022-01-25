#include "editor/Editor.h"

int main()
{
    Editor::Options options;

    options.enableViewports = true;
    GLFWwindow* window = Editor::createWindow("kebab", 800, 800, options);
    Editor::WidgetStore widgetStore;
    Editor::TextureStore textureStore;

    if (auto tex = Editor::Gfx::loadTexture("scot.png"))
        textureStore.push_back(std::move(tex.value()));

    Editor::init(window, options, widgetStore, textureStore);
    Editor::renderLoop(window, widgetStore, textureStore);

    Editor::destroyWindow(window);
}