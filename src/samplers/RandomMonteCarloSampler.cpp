#include "RandomMonteCarloSampler.h"
#include "core/utility.h"
#include "materials/Material.h"

void RandomMonteCarloSampler::render(int samples)
{
    // Filling the queue
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            Task task {x,y, 1, 1, 0, samples};
            threadpool.push(std::move(task));
        }
    }

    // Do the actual rendering
    threadpool.spawnThreads();
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

    const int dim = 3;
    const int idx = xyToIdx(x, y, dim, m_width);
    for (int i = 0; i < dim; ++i)
    {
        (*m_colorbuffer)[idx + i] += color[i];
    }
}
