/** @file */

#ifndef RAYTRACER_ENGINE_H
#define RAYTRACER_ENGINE_H

#include <memory>
#include "primitives/Primitive.h"
#include "Vector3D.h"
#include "Camera.h"

using namespace std;

/** @brief Scatters rays recursively based on material it hits.
 *  @details Calls Material member function Scatter() recursively.
 *  @param r that is tested with hit equation
 *  @param world contains reference to array of all primitives (Primitivelist)
 *  @param depth is used by recursion to determine current depth. This should be
 *  set to 0 when function is called first time.
 *  @param randomFloat contains reference to random generator.
 *  @param depthlimit is depth limit for recursion. This is compared against depth
 *  parameter. */
Vector3D recursiveScatter(const Ray &r, Primitive *world, int depth, function<float()> &randomFloat,
                          const int depthlimit);

/** @brief Engine class handles the rendering, writing
 * rendered pixels to framebuffer and storing
 * framebuffer to disk.
 * @details framebuffer is array which stores all rendered pixels in heap.
 * When framebuffer is stored on disk, netpbm format is used see
 * https://en.wikipedia.org/wiki/Netpbm. */

class Engine {

private:
    Primitive *world;
    unique_ptr<unique_ptr<Vector3D[]>[]> framebuffer;
    Camera camera;
    int width;
    int height;
    int depthlimit;

public:
    /** @brief Constructor initializes framebuffer and object member variables
     *  @param world is pointer to array which contains all primitives in the world */
    Engine(Primitive *world, const Camera &camera, int width, int height);

    /** @brief computes color for every pixel on screen.
     *  @details Multithreading is done inside this member function.
     *  @param samples number of times color is computed for each pixel and
     *  then averaged. This affects quality of supersampling and ray
     *  scattering. */
    void render(int samples);

    void frammebufferToNetpbm(string filename);

    /** @brief sets recursion depth for recursiveScatter() */
    void setDepthLimit(int limit);

};


#endif //RAYTRACER_ENGINE_H
