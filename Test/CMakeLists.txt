project(Googletest)

add_subdirectory(lib/googletest)

set(SRC ${CMAKE_SOURCE_DIR}/Source)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../Source)

add_executable(gtest_run Vector3DTest.cpp ../Source/Ray.cpp ../Source/Camera.cpp ../Source/Utility.cpp ../Source/primitives/Sphere.cpp ../Source/primitives/Primitivelist.cpp ../Source/primitives/Primitivelist.h ../Source/materials/Material.h ../Source/materials/Lambertian.cpp ../Source/materials/Lambertian.h CameraTest.cpp RayTest.cpp LambertianTest.cpp ../Source/materials/Metal.cpp ../Source/materials/Metal.h ../Source/materials/Brdf.cpp ../Source/materials/Brdf.h ../Source/primitives/Aabb.cpp EngineTest.cpp)
target_link_libraries(gtest_run gtest gtest_main)