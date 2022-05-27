include_directories(.)

set(RAYTRACER_SRC core/Ray.cpp
    core/Camera.cpp
    core/Vector3D.cpp
    core/utility.cpp
    primitives/Sphere.cpp
    primitives/Hitlist.cpp
    primitives/Aabb.cpp
    primitives/Bvhnode.cpp
    primitives/Triangle.cpp
    primitives/Mesh.cpp
    materials/Lambertian.cpp
    materials/Metal.cpp
    materials/Dielectric.cpp
    materials/Mix.cpp
    samplers/RandomMonteCarloSampler.cpp
    samplers/FastSampler.cpp
    threads/Threadpool.cpp
    logging/Logging.cpp
    io/Io.cpp
    io/MeshLoader.cpp
    scene/Components.cpp
)

set(RAYTRACER_GUI
    editor/Editor.cpp
    editor/Widgets.cpp
    editor/Graphics.cpp
    editor/Input.cpp
    editor/Windowing.cpp
    ../external/ImFileDialog/ImFileDialog.cpp
    ${DEAR_IMGUI_SOURCE})

add_executable(raytracer core/main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
target_link_libraries(raytracer ${EXTERNAL})