#ifndef RAYTRACER_WINDOW_H
#define RAYTRACER_WINDOW_H

#define GLEW_STATIC
#include <GLFW/glfw3.h>
#include "core/Raytracer.h"

class Window {

private:
    GLFWwindow *window;
    void render() const;

public:
    /**
     * @brief Handles window creation and
     * displaying Gui.
     */
    Window(const char *title);
    ~Window();
};

#endif //RAYTRACER_WINDOW_H
