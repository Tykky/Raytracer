#include "editor/Editor.h"

int main()
{
    GLFWwindow* window = Editor::createWindow(800, 800, "kebab");
    Editor::Options options{};
    options.enableViewports = true;
    Editor::init(window, options);
    Editor::renderLoop(window);

    Editor::destroyWindow(window);
}