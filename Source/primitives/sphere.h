#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "vector3D.h"
#include "primitive.h"

class sphere: public primitive {
private:
    vector3D center;
    float radius;
public:
    sphere(const vector3D &center, float radius);
    bool hit(const ray &r, float cmin, float cmax, hitrecord &record) const override;
};


#endif //RAYTRACER_SPHERE_H
