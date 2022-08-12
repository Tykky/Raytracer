#include "editor/Editor.h"
#include "editor/Graphics.h"
#include "logging/Logging.h"
#include "scene/Components.h"
#include "scene/Scene.h"
#include "util/Math.h"

int main()
{
    Editor::Options options;
    initLogger(128);

    options.enableViewports = true;
    options.enableMainWindowBorders = true;
    options.enableVsync = false;

    Matrix<u32, 4, 4> m = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    auto s = view(m);

    /*
    Editor::createEditorWindow("", 1920, 1080, options);
    Editor::init(options);
    Editor::renderLoop();
    Editor::destroyWindow();
    */
}