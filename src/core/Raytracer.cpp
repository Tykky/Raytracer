#include <random>
#include <vector>
#include <memory>
#include <fstream>
#include "Raytracer.h"
#include "utility.h"
#include "materials/Material.h"

Raytracer::Raytracer(Primitive* world, Camera* camera, int width, int height) :
    m_world(world),
    m_camera(camera),
    m_width(width),
    m_height(height),
    m_bouncelimit(10),
    m_isRendering(false),
    m_framebuffer(3 * width * height),
    m_colorbuffer(3 * width * height),
    m_sampleCounter(0),
    m_sampler(world, camera, &m_colorbuffer, &m_framebuffer, width, height, m_bouncelimit)
{}

void Raytracer::render(unsigned int samples) 
{
    if(m_isRendering) 
    {
        return;
    }

    m_isRendering = true;

    m_sampler.render(samples);

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

Framebuffer& Raytracer::getFramebuffer()
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

void Raytracer::setWorld(Primitive* world)
{
    m_isRendering = false;
    m_world = world;
    m_sampler.setWorld(world);
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

void Raytracer::clearColorbuffer() 
{
    m_isRendering = false;
    std::fill(m_colorbuffer.begin(), m_colorbuffer.end(), 0);
}