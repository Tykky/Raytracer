#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H

#include "Material.h"
#include "functional"

/** @brief Lambertian material is "matte" or diffusely reflecting Material. */
class Lambertian : public Material {
private:
    const Vector3D albedo;
public:
    Lambertian(const Vector3D &albedo);

    virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_LAMBERTIAN_H
