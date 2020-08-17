#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H

#include "Material.h"
#include "functional"

/** @brief Lambertian material is "matte" or diffusely reflecting Material. */
class Lambertian : public Material {

public:
    Lambertian(const Vector3D &albedo);

    virtual bool scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;

private:
    const Vector3D albedo;

};


#endif //RAYTRACER_LAMBERTIAN_H
