include_directories(.)

if(WIN32)
    set(win32 "WIN32")
endif()

add_executable(raytracer ${win32} 
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
               gui/Mainwindow.cpp)

target_link_libraries(raytracer PRIVATE Qt5::Widgets OpenMP::OpenMP_CXX)
