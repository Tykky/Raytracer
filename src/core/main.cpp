#include "editor/Editor.h"
#include "io/Io.h"
#include <iostream>

int main()
{
    Editor::Options options;

    options.enableViewports = true;
    GLFWwindow* window = Editor::createWindow("kebab", 800, 800, options);
    Editor::WidgetStore widgetStore;
    Editor::TextureStore textureStore;

    if (auto tex = Editor::loadTexture("scot.png"))
        textureStore.push_back(std::move(tex.value()));

    Editor::init(window, options, widgetStore, textureStore);
    Editor::renderLoop(window, widgetStore, textureStore);
    Editor::destroyWindow(window);
}