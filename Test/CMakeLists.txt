project(Googletest)

add_subdirectory(lib/googletest)

set(SRC ${CMAKE_SOURCE_DIR}/Source)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../Source)

add_executable(gtest_run vector3Dtest.cpp ../Source/vector3D.cpp)
target_link_libraries(gtest_run gtest gtest_main)