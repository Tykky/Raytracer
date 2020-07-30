#ifndef RAYTRACER_WINDOW_H
#define RAYTRACER_WINDOW_H

#define GLEW_STATIC
#include <GLFW/glfw3.h>
#include "core/Raytracer.h"

class Window {

private:
    std::shared_ptr<Raytracer> raytracer;
    GLFWwindow *window;
    int width;
    int height;
    void render() const;

public:
    Window(int width, int height, const char *title, std::shared_ptr<Raytracer> raytracer);
    ~Window();
};


#endif //RAYTRACER_WINDOW_H
