#ifndef RAYTRACER_ENGINE_H
#define RAYTRACER_ENGINE_H

#include <memory>
#include <functional>
#include "hittables/Hittable.h"
#include "Vector3D.h"
#include "Camera.h"

class Raytracer {

public:
    /**
     * @param [in] world is data structure that returns closest
     * hit when .hit() is called, i.e Hitlist or Bvhnode.
     * @param [in] camera
     * @param [in] width
     * @param [in] height
     */
    Raytracer(std::shared_ptr<Hittable> world, const Camera &camera, int width, int height);

    /**
     * @brief computes color for every pixel on the screen.
     * @param [in] samples
     */
    void render(int samples);

	/**
	 * @brief stores framebuffer data to .ppm file
	 * @param [in] filename
	 */
    void frammebufferToNetpbm(std::string filename);

    std::vector<unsigned char> &getFramebuffer();
    void clearFramebuffer();

    /** @param [in] bouncelimit */
    void setBounceLimit(int bouncelimit);
    /** @param [in] width */
    void setWidth(int width);
    /** @param [in] height */
    void setHeight(int height);
    /** @param [in] camera */
    void setCamera(Camera &camera);
    /** @param [in] world */
    void setWorld(std::shared_ptr<Hittable> world);

private:
    std::shared_ptr<Hittable> world_;
    std::vector<unsigned char> framebuffer_;
    std::vector<float> colorbuffer_;
    Camera camera_;
    int width_;
    int height_;
    int bouncelimit_;

    /**
     *  @brief Computers color for Ray r
	 *  @param [in] r is being traced
	 *  @param [in] randomFloat contains reference to random generator.
     */
    Vector3D rayTrace(Ray& r, std::function<float()> &randomFloat) const;


};


#endif //RAYTRACER_ENGINE_H
