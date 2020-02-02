/** primitive is abstract class which provides interface for all primitives in the world. */

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "ray.h"
#include "vector3D.h"

struct hitRecord {
    float c;
    vector3D p;
    vector3D normal;
};


class object {
public:
    /**@brief returns true when the object is hit by ray r.
     * @param cmin is minimum acceptable value for hit equation's solution.
     * @param cmax is maximum acceptable value for hit equation's solution.
     * hit equation is different for all primitives. */
    virtual bool hit(const ray &r, float cmin, float cmax, hitRecord rec);
};


#endif //RAYTRACER_OBJECT_H
