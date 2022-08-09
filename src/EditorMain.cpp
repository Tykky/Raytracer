#include "editor/Editor.h"
#include "editor/Graphics.h"
#include "logging/Logging.h"
#include "scene/Components.h"
#include "scene/Scene.h"
#include "util/Math.h"

void test()
{
    Vec<float, 4> v = {2.5, 3.4, 2.5, 2.1};
    Vec<float, 4> a = {2.5, -3.4, -2.5, 2.1};
    auto b = cross(v,a);
    auto d = dot(a, b);

    Matrix<float, 4, 4> m1 = {1, 2, 3, 4,
                              3, 4, 5, 6,
                              7, 8, 9, 10,
                              11, 12, 13 ,14};

    Matrix<float, 4, 4> m2 = {1, 2, 3, 4,
                              3, 4, 5, 6,
                              7, 8, 9, 10,
                              11, 12, 13 ,14};


    auto m12 = m1 * m2;

    RT_LOG_MSG("vector is {}", v.to_string());
    RT_LOG_MSG("dotis {}", d);
    RT_LOG_MSG("matrix is {}", m1.to_string());
    RT_LOG_MSG("transpose is {}", m1.transpose().to_string());
    RT_LOG_MSG("matrix product is {}", m12.to_string());

}

int main()
{
    Editor::Options options;
    initLogger(128);

    test();

    options.enableViewports = true;
    options.enableMainWindowBorders = true;
    options.enableVsync = false;
    //Editor::createEditorWindow("", 1920, 1080, options);
    //RT::addComponents<RT::TransformComponent, RT::CameraComponent>(scene, entity);

    //Editor::init(options);
    //Editor::renderLoop();

    //Editor::destroyWindow();
}