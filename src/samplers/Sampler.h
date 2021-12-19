#ifndef RAYTRACER_SAMPLER_H
#define RAYTRACER_SAMPLER_H

#include <random>
#include <functional>

#include "core/Vector3D.h"
#include "core/Ray.h"
#include "core/buffers.h"
#include "hittables/Primitive.h"
#include "core/Camera.h"

class Sampler;

#include "threads/Threadpool.h"

class Sampler
{
public:
    Sampler(Primitive* world, Camera* camera, Colorbuffer* colorbuf, Framebuffer* framebuf, int width, int height) :
        m_world(world),
        m_camera(camera),
        m_colorbuffer(colorbuf),
        m_framebuffer(framebuf),
        m_width(width),
        m_height(height)
    {}

    virtual void render(int samples) = 0;
    virtual void samplePixel(int x, int y) = 0;

    inline void setWorld(Primitive* world) { m_world = world; }
    inline void setCamera(Camera* camera) { m_camera = camera; }
    inline void setColorbuffer(Colorbuffer* colorbuf) { m_colorbuffer = colorbuf; }

protected:
    Primitive*    m_world;
    Camera*      m_camera;
    Colorbuffer* m_colorbuffer;
    Framebuffer* m_framebuffer;
    Taskbuffer   m_taskbuffer;
    unsigned int m_width;
    unsigned int m_height;
};

#endif //RAYTRACER_SAMPLER_H