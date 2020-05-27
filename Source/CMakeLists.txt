include_directories(.)

if(WIN32)
    set(win32 "")
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
               primitives/Bvhnode.cpp)

target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)
