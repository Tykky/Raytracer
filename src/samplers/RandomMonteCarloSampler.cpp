#include "RandomMonteCarloSampler.h"
#include "core/utility.h"
#include "materials/Material.h"
#include <cassert>

void RandomMonteCarloSampler::render(int samples)
{
    // Filling the queue and taskbuffer
    m_taskbuffer.resize(m_width * m_height);
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            int idx = xyToIdx(x, y, 1, m_width);
            m_taskbuffer[idx] = {x, y, x + 1, y + 1, 0, samples};
            m_threadpool.push(&m_taskbuffer[idx]);
        }
    }

    // Do the actual rendering
    m_threadpool.spawnThreads();
}

void RandomMonteCarloSampler::samplePixel(int x, int y)
{
    int depth = 0;
    Hitrecord record;
    const float epsilon = 0.001f;
    Vector3D color(1, 1, 1);

    Ray r = m_camera->getRay(
      (static_cast<float>(x) + m_randomFloat()) / static_cast<float>(m_width),
      (static_cast<float>(y) + m_randomFloat()) / static_cast<float>(m_height)
    );

    while(m_world && m_world->hit(r, epsilon, std::numeric_limits<float>::max(), record))
    {
        Ray scatter;
        Vector3D attenuation;
        if (depth < m_bounceLimit && record.matptr->scatter(r, record, attenuation, scatter, m_randomFloat))
        {
            depth++;
            r = scatter;
            color *= attenuation;
        }
        else
        {
            color = {0, 0, 0};
        }
    }

    if (depth == 0)
    {
        color = skyGradient(r);
    }
    const int taskIdx = xyToIdx(x, y, 1, m_width);
    Task& task = m_taskbuffer[taskIdx];

    task.sampleCount++;

    const int colorIdx = xyToIdx(x, y, 3, m_width);
    for (int i = 0; i < 3; ++i)
    {
        (*m_colorbuffer)[colorIdx + i] += color[i];
        const float totalColor = (*m_colorbuffer)[colorIdx + i];
        const int framebufferColor = static_cast<int>((totalColor / task.sampleCount) * 255.99);
        assert(adjustedColor <= 256);
        (*m_framebuffer)[colorIdx + i] = static_cast<unsigned char>(framebufferColor);
    }
}