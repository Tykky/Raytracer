include_directories(.)

set(RAYTRACER_SRC
    logging/Logging.cpp
    io/Io.cpp
    scene/Scene.cpp
    mesh/Mesh.cpp
)

set(RAYTRACER_GUI
    editor/Editor.cpp
    editor/Widgets.cpp
    editor/Graphics.cpp
    ../external/ImFileDialog/ImFileDialog.cpp
    ${DEAR_IMGUI_SOURCE})

add_executable(raytracer EditorMain.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
target_link_libraries(raytracer ${EXTERNAL})