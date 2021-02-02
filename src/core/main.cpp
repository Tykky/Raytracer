#include "gui/Window.h"
#include "io/meshloader.h"
#include "materials/Material.h"
#include "materials/Lambertian.h"



int main() {
    //Window window("Raytracer");
    Lambertian mat = {{0.5f, 0.5f, 0.5f}};
    auto meshes = loadObj("cube.obj", &mat);

    return 0;
}
