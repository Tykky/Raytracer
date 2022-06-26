#include "editor/Editor.h"

int main()
{
    Editor::Options options;
    initLogger(128);

    options.enableViewports = true;
    options.enableMainWindowBorders = true;
    Editor::createEditorWindow("kebab", 800, 600, options);

    Editor::init(options);
    Editor::renderLoop();
    Editor::destroyWindow();
}