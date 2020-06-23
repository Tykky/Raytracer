#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "core/Ray.h"
#include "primitives/Primitive.h"
#include <functional>

/** @brief Abstract class which provides interface for all materials. */
class Material {
public:
    virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
