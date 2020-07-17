cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

option(BUILD_GUI "" ON)
option(BUILD_CLI "" ON)
option(BUILD_TEST "" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(OpenMP REQUIRED)
link_libraries(OpenMP::OpenMP_CXX)

if(BUILD_GUI)
	include_directories(lib/glew-cmake/include)
	add_subdirectory(lib/glfw)
	add_subdirectory(lib/glew-cmake)
	add_subdirectory(lib/glew-cmake/build/cmake)
	find_package(OpenGL REQUIRED)
endif()

if(BUILD_GUI OR BUILD_CLI)
	add_subdirectory(Source)
endif()

if(BUILD_TEST)
	add_subdirectory(Test)
endif()

