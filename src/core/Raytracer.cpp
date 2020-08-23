#include <random>
#include <vector>
#include <memory>
#include <fstream>
#include "Raytracer.h"
#include "Utility.h"
#include "materials/Material.h"

Raytracer::Raytracer(std::shared_ptr<Hittable> world, const Camera &camera, int width, int height) :
        world_(std::move(world)), camera_(camera), width_(width), height_(height), bouncelimit_(50),
        framebuffer_(std::vector<unsigned char>(3 * width * height)), colorbuffer_(std::vector<float>(3 * width * height)) {
}

void Raytracer::render(int samples) {
#pragma omp parallel
    {
        // Each thread has it's own random generator
        std::mt19937 engine(1337);
        std::uniform_real_distribution<float> dist(0.0, 1.0);
        std::function<float()> randomFloat = bind(dist, engine);

        for(int s = 1; s < samples; s++) {
            #pragma omp for collapse(2) schedule(static, 10)
            for (int y = height_ - 1; y >= 0; --y) {
                for (int x = 0; x < width_; ++x) {
                    Ray r = camera_.getRay((float(x) + randomFloat()) / float(width_),
                                          (float(y) + randomFloat()) / float(height_));
                    int i = 3 * (width_ * height_ - (width_ * y + x)) - 3;
                    Vector3D color = rayTrace(r, randomFloat);
                    colorbuffer_[i] += color.getR();
                    colorbuffer_[i + 1] += color.getG();
                    colorbuffer_[i + 2] += color.getB();
                    color = Vector3D(colorbuffer_[i] / s, colorbuffer_[i + 1] / s, colorbuffer_[i + 2] / s);
                    // Write gamma corrected pixels to framebuffer
                    // Data format: [R, G, B, R, G, B, ...]
                    // stride = 3 bytes
                    framebuffer_[i + 0] = static_cast<int>(std::sqrt(color.getR()) * 255.99);
                    framebuffer_[i + 1] = static_cast<int>(std::sqrt(color.getG()) * 255.99);
                    framebuffer_[i + 2] = static_cast<int>(std::sqrt(color.getB()) * 255.99);
                }
            }
        }
    }
}

void Raytracer::frammebufferToNetpbm(std::string filename) {
    std::ofstream of;
    of.open(filename + ".ppm");
    of << "P3\n" << width_ << " " << height_ << "\n255\n";
    for (int y = height_ - 1; y >= 0; --y) {
        for (int x = 0; x < width_; ++x) {
            int i = 3 * (width_ * height_ - (width_ * y + x)) - 3;
            of << static_cast<int>(framebuffer_[i + 0]) << " ";
            of << static_cast<int>(framebuffer_[i + 1]) << " ";
            of << static_cast<int>(framebuffer_[i + 2]) << "\n";
        }
    }
    of.close();
}

std::vector<unsigned char>& Raytracer::getFramebuffer() {
    return framebuffer_;
}

void Raytracer::clearFramebuffer() {
    int framebuffer_size = 3 * width_ * height_;
    for (int i = 0; i < framebuffer_size; ++i) {
        framebuffer_[i] = 0;
    }
}

void Raytracer::setBounceLimit(int bouncelimit) {
    bouncelimit_ = bouncelimit;
}

void Raytracer::setWidth(int width) {
    width_ = width;
    clearFramebuffer();
}

void Raytracer::setHeight(int height) {
    height_ = height;
    clearFramebuffer();
}

void Raytracer::setCamera(Camera &camera) {
    camera = camera;
}

void Raytracer::setWorld(std::shared_ptr<Hittable> world) {
    world_ = std::move(world);
}

Vector3D Raytracer::rayTrace(Ray& r, std::function<float()> &randomFloat) const {

    int depth = 0;
    Hitrecord record;
    const float epsilon = 0.001f;
    Vector3D color(1, 1, 1);

    while (world_ && world_->hit(r, epsilon, std::numeric_limits<float>::max(), record)) {
        Ray scatter;
        Vector3D attenuation;
        if (depth < bouncelimit_ && record.matptr->scatter(r, record, attenuation, scatter, randomFloat)) {
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

