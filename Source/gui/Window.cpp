#include <GL/glew.h>
#include <thread>
#include "Window.h"
#include "Gui.h"

void error_callback(const int error, const char* description) {
  std::cerr << "[ERROR] " << error << " " << description << std::endl;
}

Window::Window(const char *title) {
    
    glfwSetErrorCallback(error_callback);
	
    if (glfwInit()) {
        glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
    	window = glfwCreateWindow(800, 600, title, NULL, NULL);
    	if(!window) {
            glfwTerminate();
            throw std::runtime_error("[ERROR] glfwCreateWindow failed");
        }
        glfwMakeContextCurrent(window);
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("[ERROR] glewInit failed");
        }
        render();
    } else {
        throw std::runtime_error("[ERROR] glfwinit failed");
    }
}

Window::~Window() {
    glfwDestroyWindow(window);
}

void Window::render() const {

   // disable vsync
   //glfwSwapInterval(0);

   Gui gui(window);
   gui.init(); // Creates OpenGl context etc...

   // Main render loop
   while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        gui.renderGui();
        glClear(GL_COLOR_BUFFER_BIT);
        gui.renderDrawData();
        glfwSwapBuffers(window);
   }

}