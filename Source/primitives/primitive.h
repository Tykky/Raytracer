/** primitive is abstract class which provides interface for all primitives in the world. */

#ifndef RAYTRACER_PRIMITIVE_H
#define RAYTRACER_PRIMITIVE_H

#include "ray.h"

struct hitrecord {
    float c; // solution to hit equation
    vector3D p; // hit point p
    vector3D normal; // normal from hit point p
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
