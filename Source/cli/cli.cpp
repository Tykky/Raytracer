#include <iostream>
#include <chrono>
#include <random>
#include "core/Vector3D.h"
#include "core/Camera.h"
#include "primitives/Sphere.h"
#include "primitives/Primitivelist.h"
#include "materials/Lambertian.h"
#include "materials/Dielectric.h"
#include "materials/Mix.h"
#include "core/Raytracer.h"
#include "primitives/Bvhnode.h"


int main(int argc, char** argv) {

    int width = 800;
    int height = 600;
    int samples = 10;
    int fov = 90;
    int scene = 0;

    std::cout << "Enter image width: ";
    std::cin >> width;
    std::cout << "Enter image height: ";
    std::cin >> height;
    std::cout << "Enter camera preset (0-3): ";
    std::cin >> scene;
    std::cout << "Enter fov: ";
    std::cin >> fov;
    std::cout << "Enter amount of  samples: ";;
    std::cin >> samples;

    Camera cam(fov, float(width) / float(height), Vector3D(-0.2, 0.5, 1), Vector3D(0, 0.5, -1),
        Vector3D(0, 0.5, 0));

    if (scene == 1) {
        cam = Camera(fov, float(width) / float(height), Vector3D(0, 5, 1), Vector3D(0, 0.5, -1),
            Vector3D(0, 0.5, 0));
    }

    if (scene == 2) {
        cam = Camera(fov, float(width) / float(height), Vector3D(10, 5, 5), Vector3D(0, 0.5, -1),
            Vector3D(0, 0.5, 0));
    }

    if (scene == 3) {
        cam = Camera(fov, float(width) / float(height), Vector3D(100, 30, 50), Vector3D(0, 0.5, -1),
            Vector3D(0, 0.5, 0));
    }

    const int n = 1000;

    Primitive* list[n + 5];
    Mix mat(Vector3D(0.3, 0.3, 0.8), Vector3D(0.2, 1, 0.1), 0.1, 0.1, 1, 1);
    Mix lamb(Vector3D(0.3, 0.3, 0.3), Vector3D(0.1, 0.1, 0.1), 0, 0, 0.1, 1);
    Lambertian red(Vector3D(1, 0.2, 0.2));
    Dielectric glass = Dielectric(1.5);
    Mix close(Vector3D(0.0, 0.3, 0.3), Vector3D(1, 1, 1), 0.1, 0, 1, 1);
    Mix gold(Vector3D(0.85, 0.64, 0.12), Vector3D(0.85, 0.64, 0.12), 0.9, 0.1, 0, 1);

    Material* matptr = &mat;
    Material* lambptr = &lamb;
    Material* glassptr = &glass;
    Material* redptr = &red;
    Material* closeptr = &close;

    std::mt19937 gen(456);
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    std::function<float()> randomFloat = std::bind(dist, gen);

    int multp = 15;


    for (int i = 0; i < n; ++i) {
        float randi = randomFloat();
        float radius = randomFloat();
        if (randi < 0.25) {
            list[i] = new Sphere(Vector3D(multp * randomFloat() - multp / 2, 0.1, -multp * randomFloat()), 0.1, glassptr);
        } else if (randi < 0.5) {
            list[i] = new Sphere(Vector3D(multp * randomFloat() - multp / 2, 0.1, -multp * randomFloat()), 0.1, redptr);
        } else {
            list[i] = new Sphere(Vector3D(multp * randomFloat() - multp / 2, 0.1, -multp * randomFloat()), 0.1, matptr);
        }
    }
    list[n] = new Sphere(Vector3D(0, -1000, 0), 1000, lambptr);

    list[n + 1] = new Sphere(Vector3D(0, 0.5, -1), 0.5, closeptr);
    list[n + 2] = new Sphere(Vector3D(-10, 4.7, -20), 5, matptr);
    list[n + 3] = new Sphere(Vector3D(-1.5, 0.5, -3), 0.5, closeptr);
    list[n + 4] = new Sphere(Vector3D(-1.5, 15, -3), 10, lambptr);

    Primitive* world = new Primitivelist(list, n + 5);

    Primitive* bvh = new Bvhnode(list, n + 5, 0, 1, randomFloat);

    Raytracer engine(bvh, cam, width, height);

    auto start = std::chrono::system_clock::now();
    std::cout << "rendering started.." << std::endl;

    engine.render(samples);

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "rendering finished in " << double(elapsed.count()) << " ms" << std::endl;

    start = std::chrono::system_clock::now();
    std::cout << "writing to a file... " << std::endl;
    engine.frammebufferToNetpbm("image");
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "writing finished in " << double(elapsed.count()) << " ms" << std::endl;

    return 0;
}