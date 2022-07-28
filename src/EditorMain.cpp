#include "editor/Editor.h"
#include "logging/Logging.h"
#include "scene/Components.h"
#include "scene/Scene.h"

int main()
{
    Editor::Options options;
    initLogger(128);

    options.enableViewports = true;
    options.enableMainWindowBorders = true;
    options.enableVsync = false;
    Editor::createEditorWindow("", 1920, 1080, options);

    RT::Scene scene;
    entt::entity entity = scene.registry.create();

    RT::addComponents<RT::TransformComponent, RT::CameraComponent>(scene, entity);

    vec<float, 3> asd = {1 ,2, 3};
    vec<float, 3> dj = {1 ,2, 3};

    auto b = asd + dj;
    auto c = asd - dj;
    auto g = asd + 5;

    RT_LOG_MSG("{} {} {}", b[0], b[1], b[2]);

    Editor::init(options);
    Editor::renderLoop();

    Editor::destroyWindow();
}