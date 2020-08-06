#ifndef RAYTRACER_BRDF_H
#define RAYTRACER_BRDF_H

#include <memory>
#include "Material.h"

/** @brief Combines lambertian, metal and fresnel effect in one material. */
class Mix : public Material {

private:
    std::shared_ptr<Material> metal;
    std::shared_ptr<Material> lambertian;
    const Vector3D malbedo;
    const Vector3D lalbedo;
    float metalness;
    float fresnelfactor;
    float ior;

public:
    /**
     *  @param metalness is the probability that material is treated as metal.
     *  Probability that material is treated as lambertian is 1 - metalness.
     *  When metalness = 1, the material is pure metal, when metalness = 0 it's
     *  pure lambertian.
     *  @param roughness defines accuracy of metal reflections. Value between 0 and 1
     *  should be given.
     *  @param fresnelfactor defines how strong the fresnel effect should be. Value between
     *  0 and 1 should be given, 1 being the full effect and 0 no effect.
     */
    Mix(const Vector3D &lalbedo, const Vector3D &malbedo, float metalness, float roughness, float fresnel,
         float ior);

    virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const;

};


#endif //RAYTRACER_BRDF_H
