#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H

#include "Material.h"
#include "functional"

/** @brief Lambertian material is "matte" or diffusely reflecting Material.
 *  @details The material scatters rays to random directions,
 *  producing "matte" like shading. */
class Lambertian : public Material {
private:
    const vector3D albedo;
public:
    Lambertian(const vector3D &albedo);

    virtual bool scatter(const Ray &r, const hitrecord &record, vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_LAMBERTIAN_H
