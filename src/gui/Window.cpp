#include <GL/glew.h>
#include <thread>
#include "Window.h"
#include "Gui.h"

void error_callback(const int error, const char* description) {
  std::cerr << "[ERROR] " << error << " " << description << std::endl;
}

Window::Window(const char *title) {
    
    glfwSetErrorCallback(error_callback);
	
    if(glfwInit()) {
        //glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
        window_ = glfwCreateWindow(800, 600, title, NULL, NULL);
    	if(!window_) {
            glfwTerminate();
            throw std::runtime_error("glfwCreateWindow failed");
        }
        glfwMakeContextCurrent(window_);
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("glewInit failed");
        }
        double xpos, ypos;
        glfwGetCursorPos(window_, &xpos, &ypos);
        glfwSetWindowPos(window_, xpos, ypos);
        render();
    } else {
        throw std::runtime_error("glfwinit failed");
    }
}

Window::~Window() {
    glfwDestroyWindow(window_);
}

void Window::render() const {

   // disable vsync
   //glfwSwapInterval(0);

   Gui gui(window_);
   gui.init(); // Creates OpenGl context etc...

   // Main render loop
   while(!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        gui.renderGui();
        glClear(GL_COLOR_BUFFER_BIT);
        gui.renderDrawData();
        glfwSwapBuffers(window_);
   }

}