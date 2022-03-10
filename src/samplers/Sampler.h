#ifndef RAYTRACER_SAMPLER_H
#define RAYTRACER_SAMPLER_H

#include <random>
#include <functional>

#include "core/Vector3D.h"
#include "core/Ray.h"
#include "core/buffers.h"
#include "primitives/Primitive.h"
#include "core/Camera.h"

class Sampler;

#include "threads/Threadpool.h"

class Sampler
{
public:
    Sampler(Primitive* world, RTCamera* camera, RTColorbuffer* colorbuf, RTFramebuffer* framebuf, int width, int height) :
        m_world(world),
        m_camera(camera),
        m_colorbuffer(colorbuf),
        m_framebuffer(framebuf),
        m_width(width),
        m_height(height)
    {}

    virtual void render(int samples) = 0;
    virtual void samplePixel(int x, int y) = 0;
    virtual void setRenderFinishedCallback(std::function<void()> callback) = 0;

    inline void setWorld(Primitive* world) { m_world = world; }
    inline void setCamera(RTCamera* camera) { m_camera = camera; }
    inline void setColorbuffer(RTColorbuffer* colorbuf) { m_colorbuffer = colorbuf; }

    inline void updateBuffers(int x, int y, Vector3D color, int sampleCount)
    {
        updateSample(xyToIdx(x, y, 3, m_width), m_colorbuffer, m_framebuffer, color, sampleCount);
    }


protected:
    Primitive*     m_world;
    RTCamera*      m_camera;
    RTColorbuffer* m_colorbuffer;
    RTFramebuffer* m_framebuffer;
    RTTaskbuffer   m_taskbuffer;
    unsigned int   m_width;
    unsigned int   m_height;

private:
    std::mutex m_writeBufferMutex;
};

#endif //RAYTRACER_SAMPLER_H