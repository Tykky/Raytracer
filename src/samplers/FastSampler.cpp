#include "core/buffers.h"
#include "FastSampler.h"
#include "core/utility.h"
#include "materials/Material.h"
#include <cassert>

void FastSampler::render(int samples)
{
    m_taskbuffer.resize(m_width * m_height);
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            int idx = xyToIdx(x, y, 1, m_width);
            m_taskbuffer[idx] = {x, y, 1, 1, 0, samples};
            m_threadpool.push(&m_taskbuffer[idx]);
        }
    }

    // Do the actual rendering
    m_threadpool.spawnThreads();
}

void FastSampler::samplePixel(int x, int y)
{
    int depth = 0;
    Hitrecord record;
    const float epsilon = 0.001f;
    Vector3D color(1, 1, 1);

    Ray r = m_camera->getRayScreenCoords(static_cast<float>(x), static_cast<float>(y), m_width, m_height);

    if (m_world && m_world->hit(r, epsilon, std::numeric_limits<float>::max(), record))
    {
        color *= Vector3D(0.5, 0.5, 0.5) * (-record.normal.dot(r.getDirection()));
    }
    else
    {
        color = {0, 0, 0};
    }

    const int taskIdx = xyToIdx(x, y, 1, m_width);
    Task& task = m_taskbuffer[taskIdx];

    task.sampleCount++;

    int colorIdx = 3 * m_width * m_height - xyToIdx(x, y, 3, m_width) - 3;
    updateSample(colorIdx, m_colorbuffer, m_framebuffer, color, task.sampleCount);
}

void FastSampler::setRenderFinishedCallback(std::function<void()> callback)
{
    m_threadpool.renderFinishedCallback = callback;
}
