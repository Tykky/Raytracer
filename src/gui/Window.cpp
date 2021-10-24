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
        m_window = glfwCreateWindow(800, 600, title, NULL, NULL);
    	if(!m_window) {
            glfwTerminate();
            throw std::runtime_error("glfwCreateWindow failed");
        }
        glfwMakeContextCurrent(m_window);
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("glewInit failed");
        }
        render();
    } else {
        throw std::runtime_error("glfwinit failed");
    }
}

Window::~Window() {
    glfwDestroyWindow(m_window);
}

void Window::render() const {

   // disable vsync
   //glfwSwapInterval(0);

   Gui gui(m_window);
   gui.init(); // Creates OpenGl context etc...

   // Main render loop
   while(!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        gui.renderGui();
        glClear(GL_COLOR_BUFFER_BIT);
        gui.renderDrawData();
        glfwSwapBuffers(m_window);
   }

}