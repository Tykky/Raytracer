cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

include(external/CMakeLists.txt)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(OpenGL_GL_PREFERENCE "GLVND")

add_subdirectory(src)