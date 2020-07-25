#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <thread>

void error_callback(const int error, const char* description) {
  std::cerr << "[ERROR] " << error << " " << description << std::endl;
}

Window::Window(const int width, const int height, const char *title, std::shared_ptr<Raytracer> raytracer) : 
    raytracer(raytracer), width(width), height(height) {
    
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

   unsigned int texture = setupTexture();

   // disable vsync
   glfwSwapInterval(0);

   auto invokeRaytracerRender = [](std::shared_ptr<Raytracer> raytracer, int samples) {
        raytracer->render(samples);
   };

   int samples = 100;

   // Run raytracer on separate thread
   std::thread th(invokeRaytracerRender, raytracer, samples);

   ImGui::CreateContext();
   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 130");

    // Update framebuffer to screen
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Raytracer window");
        ImGui::Image((void*)(intptr_t)texture, ImVec2(width, height));
        ImGui::End();

        ImGui::Render();

        glClear(GL_COLOR_BUFFER_BIT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raytracer->getFramebuffer());
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    th.detach();
}

unsigned int Window::setupTexture() const {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set wrap parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}

unsigned int Window::setupVertexBufferObject(const float vertices[], const unsigned int &size) const {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return VBO;
}

unsigned int Window::setupVertexArrayObject() const {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
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
        std::cerr << "[ERROR] shader 0x" << std::hex << shadertype << " compilation failed: " << infolog << std::endl;
    }
    
    return shader;
}

void Window::linkShaders(const int &shaderprogram, unsigned int shaders[], const unsigned int &size) const {
    for (int i = 0; i < size; i++) {
        glAttachShader(shaderprogram, shaders[i]);
    }
    glLinkProgram(shaderprogram);
    int success;
    char infolog[512];
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderprogram, 512, NULL, infolog);
        std::cerr << "[ERROR] shader program linking failed: " << infolog << std::endl;
    }
}
