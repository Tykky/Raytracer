/** @file */

#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "Material.h"

/** @brief Metal is highly reflective material which mirrors all rays.
 *  @details Rays are mirrored using surface normal as axis of symmetry. */
class Metal : public Material {

private:
    const Vector3D albedo;
    float blur;

public:
    /** @param b is blurriness, value in between 0 and 1
     * should be given, where 1 is heavy blurriness and
     * 0 is no blurriness at all. */
    Metal(const Vector3D &albedo, float b);

    virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_METAL_H
