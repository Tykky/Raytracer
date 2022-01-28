include_directories(.)

set(RAYTRACER_SRC core/Ray.cpp
        core/Camera.cpp
        core/Raytracer.cpp
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
        io/MeshLoader.cpp
        samplers/RandomMonteCarloSampler.cpp
        samplers/FastSampler.cpp
        threads/Threadpool.cpp
        logging/Logging.cpp
        io/Io.cpp
)
set(RAYTRACER_GUI
        editor/Editor.cpp
        editor/Widgets.cpp
        editor/Graphics.cpp
        ../external/imgui/imgui.cpp
        ../external/imgui/imgui_draw.cpp
        ../external/imgui/imgui_widgets.cpp
        ../external/imgui/imgui_demo.cpp
        ../external/imgui/imgui_tables.cpp
        ../external/imgui/backends/imgui_impl_glfw.cpp
        ../external/imgui/backends/imgui_impl_opengl3.cpp
        ../external/ImFileDialog/ImFileDialog.cpp)

if(BUILD_GUI)
    include_directories(../external/imgui ../external/imgui/backends)
    if(WIN32)
        add_executable(raytracer core/main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
    else()
        add_executable(raytracer core/main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
    endif()
        target_link_libraries(raytracer OpenGL::GL glew_s glfw)
endif()

if(BUILD_CLI)
    add_executable(raytracer_cli cli/cli.cpp ${RAYTRACER_SRC})
endif()
