#include "Window.h"
#include <iostream>
#include <iomanip>

void error_callback(const int error, const char* description) {
  std::cerr << "ERROR::" << error << " " << description << std::endl;
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

    const char *vertexsource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
  
    const char *fragmentsource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

    float vertices[] = {
        // first triangle
         0.5f,  0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f, 
        -0.5f,  0.5f, 0.0f, 
        // second triangle
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f, 
        -0.5f,  0.5f, 0.0f 
    };

   unsigned int vertexshader = setupShader(vertexsource, GL_VERTEX_SHADER);
   unsigned int fragmentshader = setupShader(fragmentsource, GL_FRAGMENT_SHADER);

   int shaderprogram = glCreateProgram();
   int shaders[] = { vertexshader, fragmentshader };
   linkShaders(shaderprogram, shaders, 2);
   glDeleteShader(vertexshader);
   glDeleteShader(fragmentshader);

   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glUseProgram(shaderprogram);

   // disable vsync
   glfwSwapInterval(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangles
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::setupTexture() const {

}

unsigned int Window::setupShader(const char *source, const unsigned int &shadertype) const {
    unsigned int shader = glCreateShader(shadertype);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infolog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infolog);
        std::cerr << "ERROR::SHADER::0x" << std::hex << shadertype << "::COMPILATION_FAILED " << infolog << std::endl;
    }
    
    return shader;
}

void Window::linkShaders(const int &shaderprogram, int shaders[], const unsigned int &size) const {
    for (size_t i = 0; i < size; i++) {
        glAttachShader(shaderprogram, shaders[i]);
    }
    glLinkProgram(shaderprogram);
    int success;
    char infolog[512];
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderprogram, 512, NULL, infolog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED " << infolog << std::endl;
    }
}
