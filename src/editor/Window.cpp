#include <GL/glew.h>
#include <thread>
#include "Window.h"
#include "Gui.h"

void error_callback(const int error, const char* description) 
{
  std::cerr << "[ERROR] " << error << " " << description << std::endl;
}

Window::Window(const char *title, GuiState& state)
{
    glfwSetErrorCallback(error_callback);
	
    if(glfwInit()) {
        glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
        m_window = glfwCreateWindow(800, 600, title, NULL, NULL);
    	if(!m_window) {
            glfwTerminate();
            throw std::runtime_error("glfwCreateWindow failed");
        }
        glfwMakeContextCurrent(m_window);
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("glewInit failed");
        }
        render(state);
    } 
    else 
    {
        throw std::runtime_error("glfwinit failed");
    }
}

Window::~Window() 
{
    glfwDestroyWindow(m_window);
}

void Window::render(GuiState& state) const
{
   // disable vsync
   glfwSwapInterval(0);

   // Main render loop
   while (!glfwWindowShouldClose(m_window)) 
   {
        glfwPollEvents();
        renderGui(state);
        glClear(GL_COLOR_BUFFER_BIT);
        renderDrawData();
        glfwSwapBuffers(m_window);
   }
}