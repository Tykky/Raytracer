#ifndef RAYTRACER_ENGINE_H
#define RAYTRACER_ENGINE_H

#include <vector>
#include <memory>
#include <functional>
#include <atomic>
#include <stdint.h>
#include "hittables/Hittable.h"
#include "Vector3D.h"
#include "Camera.h"
#include "buffers.h"
#include "samplers/RandomMonteCarloSampler.h"

class Raytracer
{
public:
    /**
     * @param world is data structure that returns closest
     * hit when .hit() is called, i.e Hitlist or Bvhnode.
     */
    Raytracer(Hittable* world, Camera* camera, int width, int height);

    void render(unsigned int samples);
    void frammebufferToNetpbm(const std::string& filename);
    Framebuffer& getFramebuffer();
    void clearFramebuffer();
    void resize(int width, int height);
    void setBounceLimit(int bouncelimit);
    void setCamera(Camera* camera);
    void setWorld(Hittable* world);
    bool isRendering();
    const std::atomic<uint64_t>& getSampleCounter();
    void resetSampleCounter();

private:
    Hittable* m_world;
    Framebuffer m_framebuffer;
    Colorbuffer m_colorbuffer;
    Camera* m_camera;
    int m_width;
    int m_height;
    int m_bouncelimit;
    std::atomic<bool> m_isRendering;
    std::atomic<uint64_t> m_sampleCounter;
    RandomMonteCarloSampler m_sampler;

    void clearColorbuffer();

};


#endif //RAYTRACER_ENGINE_H
