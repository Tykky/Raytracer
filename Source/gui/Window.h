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
