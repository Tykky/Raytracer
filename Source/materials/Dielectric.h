/** @file */

#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H

#include "Material.h"

/** @brief Fresnel coefficient approximation by Christhophe Schlick
 *  @details https://en.wikipedia.org/wiki/Schlick%27s_approximation
 *  @param cosine corresponds to cosine(theta) in formula
 *  @param ior = index of refraction (R0 in formula)
 *  @return fresnel coefficient */
float fresnel(float cosine, float ior);

/** @brief Dielectric material allows rays to pass through objects.
 *  It can simulate materials like water or glass.
 *  @details When ray enters object, it is refracted or reflected
 *  according to snells law. The material also computes fresnel
 *  with Christhophe Schlick's polynomial approximation. Fresnel
 *  is: when ray hit's object in shallow angle, it is more likely
 *  to reflect than refract. When ray hits in steep angle it is
 *  more likely to refract than reflect. */
class Dielectric : public Material {

private:
    const float ior; // ior = index of refraction

public:
    /** @param ior = index of refraction */
    Dielectric(float ior);

    virtual bool scatter(const Ray &r, const hitrecord &record, vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_DIELECTRIC_H
