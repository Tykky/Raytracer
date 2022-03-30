#include "editor/Editor.h"

int main()
{
    Editor::Options options;
    initLogger(128);

    options.enableViewports = true;
    Editor::createWindow("kebab", 1920, 1080, options);

    Editor::init(options);
    Editor::renderLoop();
    Editor::destroyWindow();
}