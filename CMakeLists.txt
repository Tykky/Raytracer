cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	add_compile_options("/EHa")
endif()

option(BUILD_GUI "" ON)
option(BUILD_CLI "" ON)
option(BUILD_TEST "" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(OpenGL_GL_PREFERENCE "GLVND")

include_directories(external/tinyobjloader)
include_directories(external/stb_image)
include_directories(external/ImFileDialog)
include_directories(external/glm)
include_directories(external)

if(BUILD_TEST)
	add_subdirectory(external/googletest)
	add_subdirectory(test)
endif()

if(BUILD_GUI)
	find_package(OpenGL REQUIRED)
	include_directories(external/glew-cmake/include)
	add_subdirectory(external/glfw)
	add_subdirectory(external/glew-cmake)
	add_subdirectory(external/glew-cmake/build/cmake)
endif()

if(BUILD_GUI OR BUILD_CLI)
	add_subdirectory(src)
endif()
