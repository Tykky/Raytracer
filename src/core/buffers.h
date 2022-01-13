#ifndef RAYTRACER_BUFFERS_H
#define RAYTRACER_BUFFERS_H

#include <vector>
#include <cassert>
#include "Vector3D.h"
#include <cmath>

struct Task;

// Colorbuffer stores the sum of
// all color values for each pixel
typedef std::vector<float> Colorbuffer;

// In framebuffer we convert colorbuffer
// to actual colors by averaging the colorbuffer
// values
typedef std::vector<unsigned char> Framebuffer;

// Contains a set of tasks that can be executed
// in parallel
typedef std::vector<Task> Taskbuffer;

// Convert (x,y) coordinate to colorbuffer or framebuffer index
inline unsigned int xyToIdx(int x, int y, unsigned int dim, unsigned int rowWidth)
{
    return dim * (rowWidth * y + x);
}

// First we write a sample to colorbuffer and then
// update framebuffer accordingly.
inline void updateSample(int idx, Colorbuffer* colorbuf, Framebuffer* framebuf, Vector3D color, int sampleCount)
{
    for (int i = 0; i < 3; ++i)
    {
        (*colorbuf)[idx + i] += color[i];
        const float totalColor = (*colorbuf)[idx + i];
        const int framebufferColor = static_cast<int>(std::sqrt(totalColor / static_cast<float>(sampleCount)) * 255.99);
        assert(framebufferColor <= 256);
        (*framebuf)[idx + i] = static_cast<unsigned char>(framebufferColor);
    }
}

#endif //RAYTRACER_BUFFERS_H
