#include "gui/Window.h"
#include "io/meshloader.h"

int main() {
    //Window window("Raytracer");
    loadObj("cube.obj");
    return 0;
}
