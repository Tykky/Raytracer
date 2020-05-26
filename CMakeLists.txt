cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3")
endif()


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)
find_package(OpenMP REQUIRED)

add_subdirectory(Source)
add_subdirectory(Test)
