include_directories(.)

add_executable(raytracer
               main.cpp
               Ray.cpp
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
               primitives/Bvhnode.cpp
               gui/window.cpp)

target_link_libraries(raytracer OpenGL::GL glfw)