#ifndef RAYTRACER_BRDF_H
#define RAYTRACER_BRDF_H

#include <memory>
#include "Material.h"
#include "Metal.h"
#include "Lambertian.h"

/** @brief Brdf combines lambertian, metal materials and fresnel effect.
 *  @details Brdf stands for bidirectional reflectance distribution function.
 *  The brdf defines how rays are scattered, just like lambertian or
 *  metal material does. This combines metal, lambertian materials and fresnel using
 *  some probability distribution. */
class Brdf : public Material {

private:
    std::unique_ptr<Material> mptr; // metal ptr
    std::unique_ptr<Material> lptr; // lambertian ptr
    const Vector3D malbedo; // metal albedo
    const Vector3D lalbedo; // lambertian albedo
    float metalness;
    float fresnelfactor;
    float ior; // refractive index

public:
    /** @param lalbedo defines albedo for lambertian.
     *  @param malbedo defines albedo for metal.
     *  @param metalness is the probability that material is treated as metal.
     *  Probability that material is treated as lambertian is 1 - metalness.
     *  When metalness = 1, the material is pure metal, when metalness = 0 it's
     *  pure lambertian.
     *  @param roughness defines accuracy of metal reflections. Value between 0 and 1
     *  should be given.
     *  @param fresnelfactor defines how strong the fresnel effect should be. Value between
     *  0 and 1 should be given, 1 being the full effect and 0 no effect. */
    Brdf(const Vector3D &lalbedo, const Vector3D &malbedo, float metalness, float roughness, float fresnel,
         float ior);

    virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;

};


#endif //RAYTRACER_BRDF_H
