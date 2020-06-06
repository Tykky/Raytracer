cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

option(BUILD_SOURCE "" ON)
option(BUILD_TEST "" ON)

find_package(OpenMP REQUIRED)
link_libraries(OpenMP::OpenMP_CXX)

if(BUILD_SOURCE)
	find_package(OpenGL REQUIRED)
	add_subdirectory(lib/glfw)
endif()


if(BUILD_SOURCE)
	add_subdirectory(Source)
endif()

if(BUILD_TEST)
	add_subdirectory(Test)
endif()

