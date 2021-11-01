include_directories(.)

set(RAYTRACER_SRC core/Ray.cpp
        core/Camera.cpp
        core/Raytracer.cpp
        core/Vector3D.cpp
        core/utility.cpp
        hittables/Sphere.cpp
        hittables/Hitlist.cpp
        hittables/Aabb.cpp
        hittables/Bvhnode.cpp
        hittables/Triangle.cpp
        hittables/Mesh.cpp
        materials/Lambertian.cpp
        materials/Metal.cpp
        materials/Dielectric.cpp
        materials/Mix.cpp
        io/meshloader.cpp
        samplers/RandomMonteCarloSampler.cpp
        threads/Threadpool.cpp
        )
set(RAYTRACER_GUI gui/Window.cpp
        gui/Gui.cpp
        ../external/imgui/imgui.cpp
        ../external/imgui/imgui_draw.cpp
        ../external/imgui/imgui_widgets.cpp
        ../external/imgui/imgui_demo.cpp
        ../external/imgui/examples/imgui_impl_glfw.cpp
        ../external/imgui/examples/imgui_impl_opengl3.cpp)

if(BUILD_GUI)
    include_directories(../external/imgui ../external/imgui/examples)
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
