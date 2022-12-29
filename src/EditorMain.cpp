#include <cstdlib>

#include "editor/Editor.h"
#include "editor/Graphics.h"
#include "logging/Logging.h"
#include "scene/Components.h"
#include "scene/Scene.h"
#include "util/Math.h"

#include "util/Types.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

int main()
{
    Editor::Options options;
    initLogger(128);

    options.enableViewports = true;
    options.enableMainWindowBorders = true;
    options.enableVsync = false;

    Editor::EditorContext ctx;

    Editor::createEditorWindow("", 1920, 1080, options, ctx);
    Editor::init(options, ctx);
    Editor::renderLoop(ctx);
    Editor::destroyWindow(ctx);
} 