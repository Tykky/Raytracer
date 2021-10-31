#ifndef RAYTRACER_RANDOMMONTECARLOSAMPLER_H
#define RAYTRACER_RANDOMMONTECARLOSAMPLER_H

#include "Sampler.h"
#include "threads/Threadpool.h"

typedef std::vector<int> SampleCounter;

class RandomMonteCarloSampler : public Sampler
{
public:
    virtual void render(int samples);

private:
    virtual void samplePixel(int x, int y);
    unsigned int m_bounceLimit = 7;
    std::mt19937 m_randomEngine = std::mt19937(130);
    std::uniform_real_distribution<float> m_randomDistribution = std::uniform_real_distribution<float>(0.0, 1.0);
    std::function<float()> m_randomFloat = bind(m_randomDistribution, m_randomEngine);

    Threadpool threadpool = Threadpool(this);
};

#endif //RAYTRACER_RANDOMMONTECARLOSAMPLER_H
