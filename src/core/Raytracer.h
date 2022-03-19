#ifndef RAYTRACER_ENGINE_H
#define RAYTRACER_ENGINE_H

#include <vector>
#include <memory>
#include <functional>
#include <atomic>
#include <stdint.h>
#include "primitives/Primitive.h"
#include "Vector3D.h"
#include "Camera.h"
#include "buffers.h"
#include "samplers/RandomMonteCarloSampler.h"
#include "samplers/FastSampler.h"

struct Raytracer
{
    RTPrimitives  primitives;
    RTAccelStruct accelStruct;
    RTFramebuffer framebuffer;
    RTColorbuffer colorbuffer;
    RTCamera      camera;
    std::unique_ptr<Sampler> sampler;
};


#endif //RAYTRACER_ENGINE_H
