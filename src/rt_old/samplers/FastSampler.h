#ifndef RAYTRACER_FASTSAMPLER_H
#define RAYTRACER_FASTSAMPLER_H

#include "samplers/Sampler.h"

class FastSampler : public Sampler
{
public:
    FastSampler(Primitive* world, RTCamera* camera, RTColorbuffer* colorbuf, RTFramebuffer* framebuf, int width, int height) :
        Sampler(world, camera, colorbuf, framebuf, width, height),
        m_threadpool(this)
    {}
    void render(int samples) override;
    void samplePixel(int x, int y) override;
    void setRenderFinishedCallback(std::function<void()> callback) override;

    Threadpool m_threadpool;
};

#endif //RAYTRACER_FASTSAMPLER_H
