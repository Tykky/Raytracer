
.. _program_listing_file_Source_gui_Window.h:

Program Listing for File Window.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_gui_Window.h>` (``Source\gui\Window.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_WINDOW_H
   #define RAYTRACER_WINDOW_H
   
   #define GLEW_STATIC
   #include "GL/glew.h"
   #include <GLFW/glfw3.h>
   
   class Window {
   
   private:
       GLFWwindow* window;
       
       void render() const;
       void setupTexture() const;
   
   public:
       Window(int width, int height, const char *title);
       ~Window();
   };
   
   
   #endif //RAYTRACER_WINDOW_H
