#ifndef RAYTRACER_ENGINE_H
#define RAYTRACER_ENGINE_H

#include <memory>
#include <functional>
#include "primitives/Primitive.h"
#include "Vector3D.h"
#include "Camera.h"

/**
 * @brief Handles the rendering, writing
 * rendered pixels to framebuffer and storing
 * framebuffer to disk.
 */

class Raytracer {

private:
    Primitive *world;
    std::unique_ptr<unsigned char[]> framebuffer;
    Camera camera;
    int width;
    int height;
    int bouncelimit;

    /**
     *  @brief Computers color for Ray r
	 *  @param r is being traced
	 *  @param world is data structure that contains all objects.
	 *  @param randomFloat contains reference to random generator.
     */
    Vector3D rayTrace(Ray& r, std::function<float()>& randomFloat) const;
	
public:
    /** @param world is pointer to array which contains all primitives in the world */
    Raytracer(Primitive *world, const Camera &camera, int width, int height);

    /** @brief computes color for every pixel on the screen. */
    void render(int samples);

	/* @brief stores frambuffer data to .ppm file */
    void frammebufferToNetpbm(std::string filename);

    unsigned char *getFramebuffer();

    void setBounceLimit(int bouncelimit);
    void setWidth(int width);
    void setHeight(int height);
    void setCamera(Camera &camera);
    void setWorld(Primitive *world);

};


#endif //RAYTRACER_ENGINE_H
