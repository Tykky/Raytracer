/** @brief primitive is abstract class which provides interface for all primitives in the world.
 *  @detail primitive is piece of geometry which is not divisible in to smaller pieces. */

#ifndef RAYTRACER_PRIMITIVE_H
#define RAYTRACER_PRIMITIVE_H

#include "ray.h"

class material;

struct hitrecord {
    float c; // solution to hit equation
    vector3D p; // hit point p
    vector3D normal; // normal from hit point p
    material *matptr;
};

class primitive {
public:
    /**@brief returns true when the primitive is hit by ray r.
     * @param cmin is minimum acceptable value for hit equation's solution.
     * @param cmax is maximum acceptable value for hit equation's solution.
     * note that hit equation is not the same for all primitives. */
    virtual bool hit(const ray &r, float cmin, float cmax, hitrecord &record) const = 0;
};


#endif //RAYTRACER_PRIMITIVE_H
