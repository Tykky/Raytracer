#ifndef RAYTRACER_WINDOW_H
#define RAYTRACER_WINDOW_H

#define GLEW_STATIC
#include <GLFW/glfw3.h>
#include "core/Raytracer.h"

class Window 
{
public:
    Window(const char *title);
    ~Window();

private:
    GLFWwindow* m_window;
    void render() const;
};

#endif //RAYTRACER_WINDOW_H
