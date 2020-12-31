#include <random>
#include <vector>
#include <memory>
#include <fstream>
#include "Raytracer.h"
#include "utility.h"
#include "materials/Material.h"

Raytracer::Raytracer(Hittable *world, Camera *camera, int width, int height) :
        world_(world), camera_(camera), width_(width), height_(height), bouncelimit_(50), is_rendering_(false),
        framebuffer_(std::vector<unsigned char>(3 * width * height)), colorbuffer_(std::vector<float>(3 * width * height)) 
{}

void Raytracer::render(unsigned int samples) 
{
    if(is_rendering_) {
        return;
    }
    is_rendering_ = true;
#pragma omp parallel
    {
        // Each thread has its own random generator
        std::mt19937 engine(1337);
        std::uniform_real_distribution<float> dist(0.0, 1.0);
        std::function<float()> randomFloat = bind(dist, engine);

        for(unsigned int s = 1; s <= samples; s++) 
        {
            #pragma omp for collapse(2) schedule(dynamic, 10)
            for (int y = height_ - 1; y >= 0; --y) 
            {
                for (int x = 0; x < width_; ++x) 
                {
                    if(is_rendering_) 
                    {
                        Ray r = camera_->getRay((static_cast<float>(x) + randomFloat()) / float(width_),
                                                (float(y) + randomFloat()) / float(height_));
                        int i = 3 * (width_ * height_ - (width_ * y + x)) - 3;
                        Vector3D color = rayTrace(r, randomFloat);
                        // Write gamma corrected pixels to framebuffer & colorbuffer
                        // Data format: [R, G, B, R, G, B, ...]
                        // stride = 3 bytes
                        colorbuffer_[i] += color.getR();
                        colorbuffer_[i + 1] += color.getG();
                        colorbuffer_[i + 2] += color.getB();
                        framebuffer_[i + 0] = static_cast<int>(std::sqrt(colorbuffer_[i]     / s) * 255.99);
                        framebuffer_[i + 1] = static_cast<int>(std::sqrt(colorbuffer_[i + 1] / s) * 255.99);
                        framebuffer_[i + 2] = static_cast<int>(std::sqrt(colorbuffer_[i + 2] / s) * 255.99);
                    }
                }
            }
        }
    }
    is_rendering_ = false;
    clearColorbuffer();
}

void Raytracer::frammebufferToNetpbm(const std::string &filename) 
{
    std::ofstream of;
    of.open(filename + ".ppm");
    of << "P3\n" << width_ << " " << height_ << "\n255\n";
    for (int y = height_ - 1; y >= 0; --y) 
    {
        for (int x = 0; x < width_; ++x) 
        {
            int i = 3 * (width_ * height_ - (width_ * y + x)) - 3;
            of << static_cast<int>(framebuffer_[i + 0]) << " ";
            of << static_cast<int>(framebuffer_[i + 1]) << " ";
            of << static_cast<int>(framebuffer_[i + 2]) << "\n";
        }
    }
    of.close();
}

std::vector<unsigned char> &Raytracer::getFramebuffer() 
{
    return framebuffer_;
}

void Raytracer::clearFramebuffer() 
{
    is_rendering_ = false;
    std::fill(framebuffer_.begin(), framebuffer_.end(), 0);
}

void Raytracer::resize(int width, int height) 
{
    is_rendering_ = false;
    width_ = width;
    height_ = height;
    framebuffer_.resize(3 * width_ * height_);
    colorbuffer_.resize(3 * width_ * height_);
    framebuffer_.clear();
}


void Raytracer::setBounceLimit(int bouncelimit) 
{
    is_rendering_ = false;
    bouncelimit_ = bouncelimit;
}

void Raytracer::setCamera(Camera *camera) 
{
    is_rendering_ = false;
    camera_ = camera;
}

void Raytracer::setWorld(Hittable *world) 
{
    is_rendering_ = false;
    world_ = world;
}

void Raytracer::haltRendering() 
{
    is_rendering_ = false;
    clearColorbuffer();
}

Vector3D Raytracer::rayTrace(Ray& r, std::function<float()> &randomFloat) const 
{
    int depth = 0;
    Hitrecord record;
    const float epsilon = 0.001f;
    Vector3D color(1, 1, 1);

    while(world_ && world_->hit(r, epsilon, std::numeric_limits<float>::max(), record)) 
    {
        Ray scatter;
        Vector3D attenuation;
        if (depth < bouncelimit_ && record.matptr->scatter(r, record, attenuation, scatter, randomFloat)) 
        {
            depth++;
            r = scatter;
            color *= attenuation;
        } else {
            return {};
        }
    }
    if (depth == 0) // ray hits void
    {
        return skyGradient(r);
    }
    return color;
}

void Raytracer::clearColorbuffer() 
{
    is_rendering_ = false;
    std::fill(colorbuffer_.begin(), colorbuffer_.end(), 0);
}

