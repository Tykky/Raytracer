#include "editor/Editor.h"
#include "io/Io.h"
#include <iostream>

int main()
{
    Editor::Options options;

    options.enableViewports = true;
    GLFWwindow* window = Editor::createWindow("kebab", 1920, 1080, options);
    Editor::WidgetStore  widgetStore;
    Editor::TextureStore textureStore;

    Editor::init(window, options, widgetStore, textureStore);
    Editor::renderLoop(window, widgetStore, textureStore);
    Editor::destroyWindow(window);
}