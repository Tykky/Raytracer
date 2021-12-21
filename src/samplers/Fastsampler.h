#ifndef RAYTRACER_FASTSAMPLER_H
#define RAYTRACER_FASTSAMPLER_H

#include "samplers/Sampler.h"

class Fastsampler : public Sampler
{
public:
    Fastsampler(Primitive* world, Camera* camera, Colorbuffer* colorbuf, Framebuffer* framebuf, int width, int height) :
        Sampler(world, camera, colorbuf, framebuf, width, height),
        m_threadpool(this)
    {}
    void render(int samples) override;
private:
    void samplePixel(int x, int y) override;

    Threadpool m_threadpool;
};

#endif //RAYTRACER_FASTSAMPLER_H
