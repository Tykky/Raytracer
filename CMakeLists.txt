cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_subdirectory(Source)
add_subdirectory(Test)