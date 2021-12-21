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
#include "samplers/Randommontecarlosampler.h"
#include "samplers/Fastsampler.h"

class Raytracer
{
public:
    /**
     * @param world is data structure that returns closest
     * hit when .hit() is called, i.e Hitlist or Bvhnode.
     */
    Raytracer(Primitive* world, Camera* camera, int width, int height);

    void render(unsigned int samples);
    void frammebufferToNetpbm(const std::string& filename);
    Framebuffer& getFramebuffer();
    void clearFramebuffer();
    void resize(int width, int height);
    void setBounceLimit(int bouncelimit);
    void setCamera(Camera* camera);
    void setWorld(Primitive* world);
    bool isRendering();
    const std::atomic<uint64_t>& getSampleCounter();
    void resetSampleCounter();

private:
    Primitive* m_world;
    Framebuffer m_framebuffer;
    Colorbuffer m_colorbuffer;
    Camera* m_camera;
    int m_width;
    int m_height;
    int m_bouncelimit;
    std::atomic<bool> m_isRendering;
    std::atomic<uint64_t> m_sampleCounter;
    Fastsampler m_sampler;

    void clearColorbuffer();

};


#endif //RAYTRACER_ENGINE_H
