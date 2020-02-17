
#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "material.h"
vector3D reflect(const vector3D &v, const vector3D &n);

class metal : public material {
private:
    vector3D albedo;
public:
    metal(const vector3D &albedo);
    virtual bool scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                         std::function<float()> &randomFloat) const;
};


#endif //RAYTRACER_METAL_H
