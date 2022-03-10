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

class Raytracer
{
public:
    /**
     * @param world is data structure that returns closest
     * hit when .hit() is called, i.e Hitlist or Bvhnode.
     */
    Raytracer(Primitive* world, RTCamera* camera, int width, int height);

    void render(unsigned int samples);
    void frammebufferToNetpbm(const std::string& filename);
    RTFramebuffer& getFramebuffer();
    void clearFramebuffer();
    void resize(int width, int height);
    void setBounceLimit(int bouncelimit);
    void setCamera(RTCamera* camera);
    void setWorld(Primitive* world);
    bool isRendering();
    const std::atomic<uint64_t>& getSampleCounter();
    void resetSampleCounter();
    void setRenderFinishedCallback(std::function<void()> callback);

private:
    Primitive* m_world;
    RTFramebuffer m_framebuffer;
    RTColorbuffer m_colorbuffer;
    RTCamera* m_camera;
    int m_width;
    int m_height;
    int m_bouncelimit;
    std::atomic<bool> m_isRendering;
    std::atomic<uint64_t> m_sampleCounter;
    FastSampler m_sampler;

    void clearColorbuffer();
};


#endif //RAYTRACER_ENGINE_H
