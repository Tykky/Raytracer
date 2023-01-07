#include <cstdlib>

#include "editor/Editor.h"
// #include "editor/Graphics.h"
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
    Editor::EditorContext ctx;

    ctx.options.enableViewports         = true;
    ctx.options.enableMainWindowBorders = true;
    ctx.options.enableVsync             = false;

    Editor::initEditor(ctx);
    Editor::createEditorWindow("Raytracer", 1920, 1080, ctx);
    Editor::initGui(ctx);
    Editor::renderLoop(ctx);
    Editor::destroyWindow(ctx);
} 