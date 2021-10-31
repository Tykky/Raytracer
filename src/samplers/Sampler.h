#ifndef RAYTRACER_SAMPLER_H
#define RAYTRACER_SAMPLER_H

#include <random>

#include "core/Vector3D.h"
#include "core/Ray.h"
#include "core/Raytracer.h"
#include "core/buffers.h"

class Sampler
{
public:
    Sampler(Hittable* world, Camera* camera, Colorbuffer* colorbuf, Framebuffer* framebuf, int width, int height) :
        m_world(world),
        m_camera(camera),
        m_colorbuffer(colorbuf),
        m_framebuffer(framebuf),
        m_width(width),
        m_height(height),
    {}

    virtual void render() = 0;
    virtual void samplePixel(int x, int y) = 0;

    void setWorld(Hittable* world) { m_world = world; }
    void setCamera(Camera* camera) { m_camera = camera; }

protected:
    Hittable* m_world;
    Camera* m_camera;
    Colorbuffer* m_colorbuffer;
    Framebuffer* m_framebuffer;
    Taskbuffer m_taskbuffer;
    unsigned int m_width;
    unsigned int m_height;
};

#endif //RAYTRACER_SAMPLER_H