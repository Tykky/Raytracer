#include "editor/Editor.h"

int main()
{
    Editor::Options options;
    initLogger(128);

    options.enableViewports = true;
    void* window = Editor::createWindow("kebab", 1920, 1080, options);

    Editor::init(window, options);
    Editor::renderLoop(window);
    Editor::destroyWindow(window);
}