/** @brief material is abstract class which provides interface for all materials.
 * @detail Materials define how rays are scattered when they hit primitives. */

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "ray.h"
#include "primitives/primitive.h"
#include <functional>

class material {
public:
    virtual bool scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                         std::function<float()> &randomFloat) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
