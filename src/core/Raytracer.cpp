#include <random>
#include <vector>
#include <memory>
#include <fstream>
#include "Raytracer.h"
#include "utility.h"
#include "materials/Material.h"

Raytracer::Raytracer(Hittable *world, Camera *camera, int width, int height) :
        m_world(world), m_camera(camera), m_width(width), m_height(height), m_bouncelimit(50), m_isRendering(false),
        m_framebuffer(std::vector<unsigned char>(3 * width * height)), m_colorbuffer(std::vector<float>(3 * width * height)), 
        m_sampleCounter(0) 
{}

void Raytracer::render(unsigned int samples) 
{
    if(m_isRendering) 
    {
        return;
    }
    m_isRendering = true;
#pragma omp parallel
    {
        // Each thread has its own random generator
        std::mt19937 engine(1337);
        std::uniform_real_distribution<float> dist(0.0, 1.0);
        std::function<float()> randomFloat = bind(dist, engine);

        for(unsigned int s = 1; s <= samples; s++) 
        {
            #pragma omp for collapse(2) schedule(static, 10)
            for (int y = m_height - 1; y >= 0; --y) 
            {
                for (int x = 0; x < m_width; ++x) 
                {
                    if(m_isRendering) 
                    {
                        Ray r = m_camera->getRay((static_cast<float>(x) + randomFloat()) / float(m_width),
                                                (float(y) + randomFloat()) / float(m_height));
                        int i = 3 * (m_width * m_height - (m_width * y + x)) - 3;
                        Vector3D color = rayTrace(r, randomFloat);
                        // Write gamma corrected pixels to framebuffer & colorbuffer
                        // Data format: [R, G, B, R, G, B, ...]
                        // stride = 3 bytes
                        m_colorbuffer[i] += color.getR();
                        m_colorbuffer[i + 1] += color.getG();
                        m_colorbuffer[i + 2] += color.getB();
                        m_framebuffer[i + 0] = static_cast<int>(std::sqrt(m_colorbuffer[i]     / s) * 255.99);
                        m_framebuffer[i + 1] = static_cast<int>(std::sqrt(m_colorbuffer[i + 1] / s) * 255.99);
                        m_framebuffer[i + 2] = static_cast<int>(std::sqrt(m_colorbuffer[i + 2] / s) * 255.99);
                        m_sampleCounter++;
                    }
                }
            }
        }
    }
    m_isRendering = false;
    clearColorbuffer();
}

void Raytracer::frammebufferToNetpbm(const std::string &filename) 
{
    std::ofstream of;
    of.open(filename + ".ppm");
    of << "P3\n" << m_width << " " << m_height << "\n255\n";
    for (int y = m_height - 1; y >= 0; --y) 
    {
        for (int x = 0; x < m_width; ++x) 
        {
            int i = 3 * (m_width * m_height - (m_width * y + x)) - 3;
            of << static_cast<int>(m_framebuffer[i + 0]) << " ";
            of << static_cast<int>(m_framebuffer[i + 1]) << " ";
            of << static_cast<int>(m_framebuffer[i + 2]) << "\n";
        }
    }
    of.close();
}

std::vector<unsigned char> &Raytracer::getFramebuffer() 
{
    return m_framebuffer;
}

void Raytracer::clearFramebuffer() 
{
    m_isRendering = false;
    std::fill(m_framebuffer.begin(), m_framebuffer.end(), 0);
}

void Raytracer::resize(int width, int height) 
{
    m_isRendering = false;
    m_width = width;
    m_height = height;
    m_framebuffer.resize(3 * static_cast<size_t>(m_width) * static_cast<size_t>(m_height));
    m_colorbuffer.resize(3 * static_cast<size_t>(m_width) * static_cast<size_t>(m_height));
    m_framebuffer.clear();
}


void Raytracer::setBounceLimit(int bouncelimit) 
{
    m_isRendering = false;
    m_bouncelimit = bouncelimit;
}

void Raytracer::setCamera(Camera *camera) 
{
    m_isRendering = false;
    m_camera = camera;
}

void Raytracer::setWorld(Hittable *world) 
{
    m_isRendering = false;
    m_world = world;
}

bool Raytracer::isRendering() 
{
    return m_isRendering;
}

const std::atomic<uint64_t>& Raytracer::getSampleCounter() 
{
    return m_sampleCounter;
}

void Raytracer::resetSampleCounter() 
{
    m_sampleCounter = 0;
}

Vector3D Raytracer::rayTrace(Ray& r, std::function<float()> &randomFloat) const 
{
    int depth = 0;
    Hitrecord record;
    const float epsilon = 0.001f;
    Vector3D color(1, 1, 1);

    while(m_world && m_world->hit(r, epsilon, std::numeric_limits<float>::max(), record)) 
    {
        Ray scatter;
        Vector3D attenuation;
        if (depth < m_bouncelimit && record.matptr->scatter(r, record, attenuation, scatter, randomFloat)) 
        {
            depth++;
            r = scatter;
            color *= attenuation;
        } else {
            return {};
        }
    }
    if (depth == 0) 
    {
        return skyGradient(r);
    }
    return color;
}

void Raytracer::clearColorbuffer() 
{
    m_isRendering = false;
    std::fill(m_colorbuffer.begin(), m_colorbuffer.end(), 0);
}

