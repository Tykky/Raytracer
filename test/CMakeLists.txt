project(Googletest)

set(SRC ${CMAKE_SOURCE_DIR}/Source)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../src)

add_executable(gtest_run Vector3DTest.cpp
        ../src/core/Ray.cpp
        ../src/core/Camera.cpp
        ../src/hittables/Sphere.cpp
        ../src/hittables/Hitlist.cpp
        ../src/materials/Lambertian.cpp
              CameraTest.cpp
              RayTest.cpp
              LambertianTest.cpp
        ../src/materials/Metal.cpp
        ../src/materials/Mix.cpp
        ../src/hittables/Aabb.cpp
        ../src/core/Raytracer.cpp
        ../src/hittables/Bvhnode.cpp
              MetalTest.cpp
              DielectricTest.cpp
        ../src/materials/Dielectric.cpp
              PrimitivelistTest.cpp
              BvhnodeTest.cpp
        ../src/core/Vector3D.cpp
        ../src/core/utility.cpp
        ../src/hittables/Triangle.cpp
        TriangleTest.cpp
        AabbTest.cpp
        ../src/hittables/Aabb.cpp SphereTest.cpp)
target_link_libraries(gtest_run gtest gtest_main)