#include "editor/Editor.h"

int main()
{
    Editor::Options options{};

    MessageStore messageStore;

    options.enableViewports = true;
    GLFWwindow* window = Editor::createWindow("kebab", 800, 800, options, messageStore);
    Editor::WidgetStore widgetStore;
    Editor::TextureStore textureStore;

    Editor::init(window, options, widgetStore, textureStore);
    Editor::renderLoop(window, widgetStore);

    Editor::destroyWindow(window);
}