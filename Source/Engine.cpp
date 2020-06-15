#include <random>
#include <functional>
#include <fstream>
#include "Engine.h"
#include "Utility.h"
#include "materials/Material.h"
#include "omp.h"

Vector3D recursiveScatter(const Ray &r, Primitive *world, int depth, std::function<float()> &randomFloat,
                          const int depthlimit) {
    hitrecord record;
    const float floaterror = 0.001;

    if (world->hit(r, floaterror, std::numeric_limits<float>::max(), record)) {
        Ray scatter;
        Vector3D attenuation;
        if (depth < depthlimit && record.matptr->scatter(r, record, attenuation, scatter, randomFloat)) {
            return attenuation * recursiveScatter(scatter, world, depth + 1, randomFloat, depthlimit);
        }
        return {};
    }
    return skyGradient(r);
}

Engine::Engine(Primitive *world, const Camera &camera, int width, int height) :
        world(world), camera(camera), width(width), height(height), depthlimit(50) {
    framebuffer = std::make_unique<std::unique_ptr<Vector3D[]>[]>(height);
    for (int i = 0; i < height; ++i) {
        framebuffer[i] = std::make_unique<Vector3D[]>(width);
    }
}

void Engine::render(int samples) {
#pragma omp parallel
    {
        // Each thread has it's own random generator
        std::mt19937 engine(1337);
        std::uniform_real_distribution<float> dist(0.0, 1.0);
        std::function<float()> randomFloat = bind(dist, engine);
#pragma omp critical
        std::cout << "Thread " << omp_get_thread_num() << " initialized" << std::endl;

#pragma omp for
        // Loop through every pixel on screen
        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                Vector3D color = Vector3D();
                // Monte carlo sampling + supersampling
                for (int s = 0; s < samples; s++) {
                    Ray r = camera.getRay((float(x) + randomFloat()) / float(width),
                                          (float(y) + randomFloat()) / float(height));
                    hitrecord record;
                    if (world->hit(r, 0, std::numeric_limits<float>::max(), record)) {
                        color += recursiveScatter(r, world, 0, randomFloat, depthlimit);
                    } else {
                        color += skyGradient(r);
                    }
                }
                color /= float(samples);
                // Write gamma corrected pixels to framebuffer
                framebuffer[y][x] = Vector3D(sqrt(color.getR()), sqrt(color.getG()), sqrt(color.getB()));
            }
        }
    }
}

void Engine::frammebufferToNetpbm(std::string filename) {
    std::ofstream of;
    of.open(filename + ".ppm");
    of << "P3\n" << width << " " << height << "\n255\n";
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            of << int(framebuffer[y][x].getR() * 255.99) << " "
               << int(framebuffer[y][x].getG() * 255.99) << " "
               << int(framebuffer[y][x].getB() * 255.99) << "\n";
        }
    }
    of.close();
}

void Engine::setDepthLimit(int limit) {
    depthlimit = limit;
}