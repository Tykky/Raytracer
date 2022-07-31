#include "editor/Editor.h"
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
    //Editor::createEditorWindow("", 1920, 1080, options);


    //RT::addComponents<RT::TransformComponent, RT::CameraComponent>(scene, entity);

    Vec<float, 3> asd = {1 ,2, 3};
    Vec<float, 3> dj = {4 ,5, 6};

    auto b = asd + dj;
    auto c = asd - dj;
    auto g = 5 + asd * 5;

    b += c;

    Vec<int, 3> pj = {4, 2, 0};

    auto mega = cross(asd, dj);
    auto dotty = dot(mega, asd);


    RT_LOG_MSG("{} {} {}", pj[0], pj[1], pj[2]);
    RT_LOG_MSG("{} {} {}", mega[0], mega[1], mega[2]);
    RT_LOG_MSG("dotty: {}", dotty);

    //Editor::init(options);
    //Editor::renderLoop();

    //Editor::destroyWindow();
}