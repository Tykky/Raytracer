#include "editor/Editor.h"
#include "io/Io.h"
#include "scene/Components.h"
#include <iostream>

int main()
{
    Editor::Options options;
    initLogger(128);

    options.enableViewports = true;
    GLFWwindow* window = Editor::createWindow("kebab", 1920, 1080, options);

    Editor::init(window, options);
    Editor::renderLoop(window);
    Editor::destroyWindow(window);
}