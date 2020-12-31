#include <iostream>
#include <chrono>
#include <random>
#include "core/Vector3D.h"
#include "core/Camera.h"
#include "hittables/Sphere.h"
#include "hittables/Hitlist.h"
#include "materials/Lambertian.h"
#include "materials/Dielectric.h"
#include "materials/Mix.h"
#include "core/Raytracer.h"
#include "hittables/Bvhnode.h"

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

    std::vector<std::shared_ptr<Hittable>> list(n + 5);
    std::shared_ptr<Material> matptr = std::make_shared<Mix>(Vector3D(0.3, 0.3, 0.8), Vector3D(0.2, 1, 0.1), 0.1, 0.1, 1, 1);
    std::shared_ptr<Material> lambptr = std::make_shared<Mix>(Vector3D(0.3, 0.3, 0.3), Vector3D(0.1, 0.1, 0.1), 0, 0, 0.1, 1);
    std::shared_ptr<Material> redptr = std::make_shared<Lambertian>(Vector3D(1, 0.2, 0.2));
    std::shared_ptr<Material> glassptr = std::make_shared<Dielectric>(1.5);
    std::shared_ptr<Material> closeptr = std::make_shared<Mix>(Vector3D(0.0, 0.3, 0.3), Vector3D(1, 1, 1), 0.1, 0, 1, 1);
    Mix gold(Vector3D(0.85, 0.64, 0.12), Vector3D(0.85, 0.64, 0.12), 0.9, 0.1, 0, 1);

    std::mt19937 gen(456);
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    std::function<float()> randomFloat = std::bind(dist, gen);

    int multp = 15;

    for (int i = 0; i < n; ++i) {
        float randi = randomFloat();
        float radius = randomFloat();
        if (randi < 0.25) {
            list[i] = std::make_shared<Sphere>(Vector3D(multp * randomFloat() - multp / 2, 0.1, -multp * randomFloat()), 0.1, glassptr.get());
        } else if (randi < 0.5) {
            list[i] = std::make_shared<Sphere>(Vector3D(multp * randomFloat() - multp / 2, 0.1, -multp * randomFloat()), 0.1, redptr.get());
        } else {
            list[i] = std::make_shared<Sphere>(Vector3D(multp * randomFloat() - multp / 2, 0.1, -multp * randomFloat()), 0.1, matptr.get());
        }
    }
    list[n] = std::make_shared<Sphere>(Vector3D(0, -1000, 0), 1000, lambptr.get());

    list[n + 2] = std::make_shared<Sphere>(Vector3D(0, 0.5, -1), 0.5, closeptr.get());
    list[n + 2] = std::make_shared<Sphere>(Vector3D(-10, 4.7, -20), 5, matptr.get());
    list[n + 3] = std::make_shared<Sphere>(Vector3D(-1.5, 0.5, -3), 0.5, closeptr.get());
    list[n + 4] = std::make_shared<Sphere>(Vector3D(-1.5, 15, -3), 10, lambptr.get());

    std::shared_ptr<Hittable> bvh = std::make_shared<Bvhnode>(list, 0, n + 4, 0, 1, randomFloat);
    //std::shared_ptr<Hittable> plist = std::make_shared<Hitlist>(list);

    Raytracer raytracer(bvh.get(), &cam, width, height);

    auto start = std::chrono::system_clock::now();
    std::cout << "rendering started.." << std::endl;

    raytracer.render(samples);

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "rendering finished in " << double(elapsed.count()) << " ms" << std::endl;

    start = std::chrono::system_clock::now();
    std::cout << "writing to a file... " << std::endl;
    raytracer.frammebufferToNetpbm("image");
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "writing finished in " << double(elapsed.count()) << " ms" << std::endl;

    return 0;
}