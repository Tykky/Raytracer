#ifndef RAYTRACER_RANDOMMONTECARLOSAMPLER_H
#define RAYTRACER_RANDOMMONTECARLOSAMPLER_H

#include "Sampler.h"

class RandomMonteCarloSampler : public Sampler
{
public:
    RandomMonteCarloSampler(Primitive* world, RTCamera* camera, RTColorbuffer* colorbuf, RTFramebuffer* framebuf, int width, int height, int bounceLimit) :
        Sampler(world, camera, colorbuf, framebuf, width, height),
        m_bounceLimit(bounceLimit),
        m_threadpool(this)
    {
        m_randomEngine = std::mt19937();
        m_randomDistribution = std::uniform_real_distribution<float>(0.0, 1.0);
        m_randomFloat = std::bind(m_randomDistribution, m_randomEngine);
    }
    void render(int samples) override;
    void samplePixel(int x, int y) override;
    void setRenderFinishedCallback(std::function<void()> callback) override;

protected:
    unsigned int m_bounceLimit;
    std::mt19937 m_randomEngine;
    std::uniform_real_distribution<float> m_randomDistribution;
    std::function<float()> m_randomFloat;
    Threadpool m_threadpool;
};

#endif //RAYTRACER_RANDOMMONTECARLOSAMPLER_H
