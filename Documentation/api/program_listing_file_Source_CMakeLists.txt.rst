
.. _program_listing_file_Source_CMakeLists.txt:

Program Listing for File CMakeLists.txt
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_CMakeLists.txt>` (``Source\CMakeLists.txt``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   include_directories(.)
   
   set(RAYTRACER_SRC core/Ray.cpp
           core/Camera.cpp
           core/Raytracer.cpp
           primitives/Sphere.cpp
           primitives/Primitivelist.cpp
           primitives/Aabb.cpp
           primitives/Bvhnode.cpp
           materials/Lambertian.cpp
           materials/Metal.cpp
           materials/Dielectric.cpp
           materials/Mix.cpp)
   set(RAYTRACER_GUI gui/Window.cpp)
   
   if(BUILD_GUI)
       if(WIN32)
           add_executable(raytracer WIN32 core/main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
       else()
           add_executable(raytracer core/main.cpp ${RAYTRACER_SRC} ${RAYTRACER_GUI})
       endif()
           target_link_libraries(raytracer OpenGL::GL glew_s glfw)
   endif()
   
   if(BUILD_CLI)
       add_executable(raytracer_cli cli/cli.cpp ${RAYTRACER_SRC})
   endif()
