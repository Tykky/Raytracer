
.. _program_listing_file_Source_gui_Window.cpp:

Program Listing for File Window.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_gui_Window.cpp>` (``Source\gui\Window.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Window.h"
   #include <iostream>
   
   void error_callback(const int error, const char* description) {
     std::cerr << "Error: " << error << " " << description << "\n";
   }
   
   Window::Window(const int width, const int height, const char *title) {
       
       glfwSetErrorCallback(error_callback);
   
       if (glfwInit()) {
           window = glfwCreateWindow(width, height, title, NULL, NULL);
           if(!window) {
               glfwTerminate();
               throw std::runtime_error("glfwCreateWindow failed");
           }
           glfwMakeContextCurrent(window);
           if (glewInit() != GLEW_OK) {
               throw std::runtime_error("glewInit failed");
           }
           render();
       } else {
           throw std::runtime_error("glfwinit failed");
       }
   }
   
   Window::~Window() {
       glfwTerminate();
       glfwDestroyWindow(window);
   }
   
   void Window::render() const {
       while (!glfwWindowShouldClose(window)) {
           glClear(GL_COLOR_BUFFER_BIT);
           glfwSwapBuffers(window);
           glfwPollEvents();
       }
   }
   
   void Window::setupTexture() const {
   
   
       
   }
