cmake_minimum_required(VERSION 3.12.4)
project(Raytracer)

set(CMAKE_CXX_STANDARD 17)

if(MSVC AND WIN32)
	# VS2022 hot reload feature requries these
	add_link_options($<$<CONFIG:Debug>:/INCREMENTAL>)
    add_compile_options($<$<CONFIG:Debug>:/ZI>)
endif()

include(external/CMakeLists.txt)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(COPY data DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(OpenGL_GL_PREFERENCE "GLVND")

add_subdirectory(src)