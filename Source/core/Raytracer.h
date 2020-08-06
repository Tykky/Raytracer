#ifndef RAYTRACER_ENGINE_H
#define RAYTRACER_ENGINE_H

#include <memory>
#include <functional>
#include "primitives/Primitive.h"
#include "Vector3D.h"
#include "Camera.h"

class Raytracer {

private:
    std::shared_ptr<Primitive> world;
    std::vector<unsigned char> framebuffer;
    Camera camera;
    int width;
    int height;
    int bouncelimit;

    /**
     *  @brief Computers color for Ray r
	 *  @param r is being traced
	 *  @param randomFloat contains reference to random generator.
     */
    Vector3D rayTrace(Ray& r, std::function<float()> &randomFloat) const;
	
public:
    /** @param world is data structure I.e primitivelist or Bvhnode */
    Raytracer(std::shared_ptr<Primitive> world, const Camera &camera, int width, int height);

    /** @brief computes color for every pixel on the screen. */
    void render(int samples);

	/** @brief stores frambuffer data to .ppm file */
    void frammebufferToNetpbm(std::string filename);

    std::vector<unsigned char>& getFramebuffer();
    void clearFramebuffer();

    void setBounceLimit(int bouncelimit);
    void setWidth(int width);
    void setHeight(int height);
    void setCamera(Camera &camera);
    void setWorld(std::shared_ptr<Primitive> world);

};


#endif //RAYTRACER_ENGINE_H
