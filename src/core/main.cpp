#include "editor/Editor.h"

int main()
{
    Editor::Options options{};

    options.enableViewports = true;
    GLFWwindow* window = Editor::createWindow("kebab", 800, 800, options);
    Editor::WidgetStore widgetStore;
    TextureStore  textureStore;

    Editor::init(window, options, widgetStore, textureStore);
    Editor::renderLoop(window, widgetStore);

    Editor::destroyWindow(window);
}