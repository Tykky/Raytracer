include_directories(.)

set(RAYTRACER_SRC core/Ray.cpp
        core/Camera.cpp
        core/Raytracer.cpp
        primitives/Sphere.cpp
        primitives/Primitivelist.cpp
        primitives/Aabb.cpp
        primitives/Bvhnode.cpp
        materials/Lambertian.cpp
        materials/Metal.cpp
        materials/Dielectric.cpp
        materials/Mix.cpp)
set(RAYTRACER_GUI gui/Window.cpp
        gui/Gui.cpp
        ../lib/imgui/imgui.cpp
        ../lib/imgui/imgui_draw.cpp
        ../lib/imgui/imgui_widgets.cpp
        ../lib/imgui/imgui_demo.cpp
        ../lib/imgui/examples/imgui_impl_glfw.cpp
        ../lib/imgui/examples/imgui_impl_opengl3.cpp)

if(BUILD_GUI)
    include_directories(../lib/imgui ../lib/imgui/examples)
    if(WIN32)
        add_executable(raytracer WIN32 core/main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
    else()
        add_executable(raytracer core/main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
    endif()
        target_link_libraries(raytracer OpenGL::GL glew_s glfw)
endif()

if(BUILD_CLI)
    add_executable(raytracer_cli cli/cli.cpp ${RAYTRACER_SRC})
endif()
