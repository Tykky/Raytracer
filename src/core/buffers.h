#ifndef RAYTRACER_BUFFERS_H
#define RAYTRACER_BUFFERS_H

#include <vector>

typedef std::vector<float> Colorbuffer;
typedef std::vector<char> Framebuffer;

inline unsigned int xyToIdx(int x, int y, unsigned int dim, unsigned int rowWidth)
{
    return dim * (rowWidth * y + x);
}

#endif //RAYTRACER_BUFFERS_H
