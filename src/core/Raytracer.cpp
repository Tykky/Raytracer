#include <random>
#include <vector>
#include <memory>
#include <fstream>
#include "Raytracer.h"
#include "Utility.h"
#include "materials/Material.h"

Raytracer::Raytracer(std::shared_ptr<Hittable> world, const Camera &camera, int width, int height) :
        world(std::move(world)), camera(camera), width(width), height(height), bouncelimit(50),
        framebuffer(std::vector<unsigned char>(3 * width * height)), colorbuffer(std::vector<float>(3 * width *height)) {
}

void Raytracer::render(int samples) {
#pragma omp parallel
    {
        // Each thread has it's own random generator
        std::mt19937 engine(1337);
        std::uniform_real_distribution<float> dist(0.0, 1.0);
        std::function<float()> randomFloat = bind(dist, engine);

        for(int s = 1; s < samples; s++) {
            #pragma omp for collapse(2) schedule(dynamic, 10)
            for (int y = height - 1; y >= 0; --y) {
                for (int x = 0; x < width; ++x) {
                    Ray r = camera.getRay((float(x) + randomFloat()) / float(width),
                                          (float(y) + randomFloat()) / float(height));
                    int i = 3 * (width * height - (width * y + x)) - 3;
                    Vector3D color = rayTrace(r, randomFloat);
                    colorbuffer[i] += color.getR();
                    colorbuffer[i + 1] += color.getG();
                    colorbuffer[i + 2] += color.getB();
                    color = Vector3D(colorbuffer[i]/s, colorbuffer[i + 1]/s, colorbuffer[i + 2]/s);
                    // Write gamma corrected pixels to framebuffer
                    // Data format: [R, G, B, R, G, B, ...]
                    // stride = 3 bytes
                    framebuffer[i + 0] = static_cast<int>(std::sqrt(color.getR()) * 255.99);
                    framebuffer[i + 1] = static_cast<int>(std::sqrt(color.getG()) * 255.99);
                    framebuffer[i + 2] = static_cast<int>(std::sqrt(color.getB()) * 255.99);
                }
            }
        }
    }
}

void Raytracer::frammebufferToNetpbm(std::string filename) {
    std::ofstream of;
    of.open(filename + ".ppm");
    of << "P3\n" << width << " " << height << "\n255\n";
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            int i = 3 * (width * height - (width * y + x)) - 3;
            of << static_cast<int>(framebuffer[i + 0]) << " ";
            of << static_cast<int>(framebuffer[i + 1]) << " ";
            of << static_cast<int>(framebuffer[i + 2]) << "\n";
        }
    }
    of.close();
}

std::vector<unsigned char>& Raytracer::getFramebuffer() {
    return framebuffer;
}

void Raytracer::clearFramebuffer() {
    int framebuffer_size = 3 * width * height;
    for (int i = 0; i < framebuffer_size; ++i) {
        framebuffer[i] = 0;
    }
}

void Raytracer::setBounceLimit(int bouncelimit) {
    this->bouncelimit = bouncelimit;
}

void Raytracer::setWidth(int width) {
    this->width = width;
    clearFramebuffer();
}

void Raytracer::setHeight(int height) {
    this->height = height;
    clearFramebuffer();
}

void Raytracer::setCamera(Camera &camera) {
    camera = camera;
}

void Raytracer::setWorld(std::shared_ptr<Hittable> world) {
    this->world = std::move(world);
}

Vector3D Raytracer::rayTrace(Ray& r, std::function<float()> &randomFloat) const {

    int depth = 0;
    Hitrecord record;
    const float epsilon = 0.001f;
    Vector3D color(1, 1, 1);

    while (world && world->hit(r, epsilon, std::numeric_limits<float>::max(), record)) {
        Ray scatter;
        Vector3D attenuation;
        if (depth < bouncelimit && record.matptr->scatter(r, record, attenuation, scatter, randomFloat)) {
            depth++;
            r = scatter;
            color *= attenuation;
        } else {
            return {};
        }
    }
    if (depth == 0) { // ray hits void
        return skyGradient(r);
    }
    return color;
}

