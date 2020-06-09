include_directories(.)

set(RAYTRACER_SRC Ray.cpp
                  Camera.cpp
                  Utility.cpp
                  primitives/Sphere.cpp
                  primitives/Primitivelist.cpp
                  materials/Lambertian.cpp
                  materials/Metal.cpp
                  materials/Dielectric.cpp
                  Engine.cpp
                  materials/Brdf.cpp
                  primitives/Aabb.cpp
                  primitives/Bvhnode.cpp)

set(RAYTRACER_GUI gui/window.cpp)

if(WIN32)
    add_executable(raytracer WIN32 main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
else()
    add_executable(raytracer main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
endif()

target_link_libraries(raytracer OpenGL::GL glew_s glfw)

add_executable(raytracer_cli cli/cli.cpp ${RAYTRACER_SRC})