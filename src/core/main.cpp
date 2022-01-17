#include "editor/Editor.h"

int main()
{
    Editor::Options options{};

    options.enableViewports = true;
    GLFWwindow* window = Editor::createWindow("kebab", 800, 800, options);

    Editor::init(window, options);
    Editor::renderLoop(window);

    Editor::destroyWindow(window);
}