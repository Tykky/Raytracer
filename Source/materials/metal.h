/** Metal is highly reflective material which mirrors
 *  all rays.*/

#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "material.h"
vector3D reflect(const vector3D &v, const vector3D &n);

class metal : public material {

private:
    const vector3D albedo;
    float blur;

public:
    metal(const vector3D &albedo, float b);
    virtual bool scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_METAL_H
