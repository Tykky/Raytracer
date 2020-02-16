project(Googletest)

add_subdirectory(lib/googletest)

set(SRC ${CMAKE_SOURCE_DIR}/Source)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../Source)

add_executable(gtest_run vector3Dtest.cpp ../Source/ray.cpp ../Source/camera.cpp ../Source/utility.cpp ../Source/primitives/sphere.cpp ../Source/primitives/primitivelist.cpp ../Source/primitives/primitivelist.h ../Source/materials/material.h ../Source/materials/lambertian.cpp ../Source/materials/lambertian.h cameratest.cpp raytest.cpp lambertiantest.cpp)
target_link_libraries(gtest_run gtest gtest_main)