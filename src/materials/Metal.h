#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "Material.h"

/** @brief Metal is highly reflective material which mirrors all rays. */
class Metal : public Material {

public:
    /**
     * @param [in] albedo is same as color
     * @param [in] b is blurriness, value in between 0 and 1
     * should be given, where 1 is heavy blurriness and
     * 0 is no blurriness at all.
     */
    Metal(const Vector3D &albedo, float b);

    virtual bool scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;

private:
    const Vector3D albedo;
    float blur;

};


#endif //RAYTRACER_METAL_H
