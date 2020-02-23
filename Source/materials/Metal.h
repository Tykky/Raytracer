/** @file */

#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "Material.h"

/** @brief Reflect function mirrors vectors using normal as axis of symmetry.
 *  @param v vector
 *  @param n normal vector */
vector3D reflect(const vector3D &v, const vector3D &n);

/** @brief Metal is highly reflective Material which mirrors all rays.
 *  @details Rays are mirrored using surface normal as axis of symmetry. */
class Metal : public Material {

private:
    const vector3D albedo;
    float blur;

public:
    Metal(const vector3D &albedo, float b);

    virtual bool scatter(const Ray &r, const hitrecord &record, vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_METAL_H
