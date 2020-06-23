#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H

#include "Material.h"

/**
 * @brief Dielectric material allows rays to pass through objects.
 * Useful for water, glass etc..
 */
class Dielectric : public Material {

private:
    const float ior;

public:
    /** @param ior = index of refraction */
    Dielectric(float ior);

    virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_DIELECTRIC_H
