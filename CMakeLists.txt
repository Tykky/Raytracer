cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

find_package(OpenMP REQUIRED)

add_subdirectory(Source)
add_subdirectory(Test)
