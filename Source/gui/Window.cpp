#include "Window.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <functional>

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
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";
  
    const char *fragmentsource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture1;"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture1, TexCoord);\n"
    "}\n\0";

    float vertices[] = {
         // position          // texture coordinates
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f
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
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);

   // generate textures
   unsigned int texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // set texture filtering parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Generate some texture data

   std::mt19937 engine(1);
   std::uniform_real_distribution<float> dist(0.0, 1.0);
   std::function<float()> randomFloat = bind(dist, engine);

   int n = 100;

   unsigned char *data = new unsigned char[3 * n * n];

   glUseProgram(shaderprogram);
   glUniform1i(glGetUniformLocation(shaderprogram, "texture1"), 0);

   // disable vsync
   glfwSwapInterval(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < 3 * n * n; i++) {
            data[i] = 255 * randomFloat();
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, n, n, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

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
    for (int i = 0; i < size; i++) {
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
