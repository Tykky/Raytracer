project(Googletest)

add_subdirectory(lib/googletest)

set(SRC ${CMAKE_SOURCE_DIR}/Source)

if(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../Source)

add_executable(gtest_run Vector3DTest.cpp
        ../Source/core/Ray.cpp
        ../Source/core/Camera.cpp
        ../Source/core/Utility.cpp
              ../Source/primitives/Sphere.cpp
              ../Source/primitives/Primitivelist.cpp
              ../Source/materials/Lambertian.cpp
              CameraTest.cpp
              RayTest.cpp
              LambertianTest.cpp
              ../Source/materials/Metal.cpp
              ../Source/materials/Brdf.cpp
              ../Source/primitives/Aabb.cpp
              EngineTest.cpp
        ../Source/core/Engine.cpp
              ../Source/primitives/Bvhnode.cpp
              MetalTest.cpp
              DielectricTest.cpp
              ../Source/materials/Dielectric.cpp
              PrimitivelistTest.cpp
              BvhnodeTest.cpp)
target_link_libraries(gtest_run gtest gtest_main)