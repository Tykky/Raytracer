/** Dielectric material allows rays to pass through objects. */

#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H

#include "material.h"

/** Fresnel coefficient approximation by Christhophe Schlick
 *  https://en.wikipedia.org/wiki/Schlick%27s_approximation */
float fresnel(float cosine, float ior);

class dielectric : public material {
private:
    const float ior;
public:
    dielectric(float ior);
    virtual bool scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                         std::function<float()> &randomFloat) const;
};



#endif //RAYTRACER_DIELECTRIC_H
