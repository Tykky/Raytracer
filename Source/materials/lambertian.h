/** @brief lambertian material is "matte" or diffusely reflecting material. */
/** @detail lambertian material scatters rays to random directions,
 *  producing "matte" like shading. */

#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H

#include "material.h"
#include "functional"

class lambertian: public material {
private:
    vector3D albedo;
public:
    lambertian(const vector3D &albedo);
    virtual bool scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_LAMBERTIAN_H
