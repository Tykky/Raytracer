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

class Raytracer {
public:
    /**
     * @param world is data structure that returns closest
     * hit when .hit() is called, i.e Hitlist or Bvhnode.
     */
    Raytracer(Hittable *world, Camera *camera, int width, int height);

    /** @brief computes color for every pixel on the screen. */
    void render(unsigned int samples);

	/** @brief stores framebuffer data to .ppm file */
    void frammebufferToNetpbm(const std::string &filename);

    std::vector<unsigned char> &getFramebuffer();
    void clearFramebuffer();
    void resize(int width, int height);
    void setBounceLimit(int bouncelimit);
    void setCamera(Camera *camera);
    void setWorld(Hittable *world);
    bool isRendering();
    const std::atomic<uint64_t>& getSampleCounter();
    void resetSampleCounter();

private:
    Hittable *world_;
    std::vector<unsigned char> framebuffer_;
    std::vector<float> colorbuffer_;
    Camera *camera_;
    int width_;
    int height_;
    int bouncelimit_;
    std::atomic<bool> is_rendering_;
    std::atomic<uint64_t> sample_counter_;
    /**
     *  @brief Computers color for Ray r
     */
    Vector3D rayTrace(Ray &r, std::function<float()> &randomFloat) const;
    void clearColorbuffer();

};


#endif //RAYTRACER_ENGINE_H
