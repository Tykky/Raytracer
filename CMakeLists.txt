cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

option(BUILD_GUI "" ON)
option(BUILD_CLI "" ON)
option(BUILD_TEST "" ON)

find_package(OpenMP REQUIRED)
link_libraries(OpenMP::OpenMP_CXX)

if(BUILD_GUI)
	include_directories(lib/glew-cmake/include)
	add_subdirectory(lib/glfw)
	add_subdirectory(lib/glew-cmake)
	add_subdirectory(lib/glew-cmake/build/cmake)
	find_package(OpenGL REQUIRED)
	add_subdirectory(Source)
endif()

if(BUILD_TEST)
	add_subdirectory(Test)
endif()

