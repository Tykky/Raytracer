#include "editor/Editor.h"
#include "editor/Graphics.h"
#include "logging/Logging.h"
#include "scene/Components.h"
#include "scene/Scene.h"
#include "util/Math.h"

void test()
{
    RT_LOG_ERROR("asd");
}

int main()
{
    Editor::Options options;
    initLogger(128);

    test();

    Vec<float, 4> v = {2.5, 3.4, 2.5, 2.1};

    Matrix<int, 4, 4> m = {0, 1, 2, 3,
                           4, 5, 6, 7,
                           8, 9, 10, 11,
                           12, 13, 14,15};

    RT_LOG_MSG("vector is {}", v.to_string());
    RT_LOG_MSG("matrix is {}", m.to_string());
    auto u = m.transpose();
    RT_LOG_MSG("matrix is {}", u.to_string());


    options.enableViewports = true;
    options.enableMainWindowBorders = true;
    options.enableVsync = false;
    //Editor::createEditorWindow("", 1920, 1080, options);
    //RT::addComponents<RT::TransformComponent, RT::CameraComponent>(scene, entity);

    //Editor::init(options);
    //Editor::renderLoop();

    //Editor::destroyWindow();
}