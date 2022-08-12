project(Googletest)

set(SRC ${CMAKE_SOURCE_DIR}/Source)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../src)

add_executable(gtest_run Vector3DTest.cpp
        ../src/core/Ray.cpp
        ../src/core/Camera.cpp
        ../src/primitives/Sphere.cpp
        ../src/primitives/Hitlist.cpp
        ../src/materials/Lambertian.cpp
              CameraTest.cpp
              RayTest.cpp
              LambertianTest.cpp
        ../src/materials/Metal.cpp
        ../src/materials/Mix.cpp
        ../src/primitives/Aabb.cpp
        ../src/core/Raytracer.cpp
        ../src/primitives/Bvhnode.cpp
              MetalTest.cpp
              DielectricTest.cpp
        ../src/materials/Dielectric.cpp
              PrimitivelistTest.cpp
              BvhnodeTest.cpp
        ../src/core/Vector3D.cpp
        ../src/core/utility.cpp
        ../src/primitives/Triangle.cpp
        TriangleTest.cpp
        AabbTest.cpp
        ../src/primitives/Aabb.cpp SphereTest.cpp)
target_link_libraries(gtest_run gtest gtest_main)