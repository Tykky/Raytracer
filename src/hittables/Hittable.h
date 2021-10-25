#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include <memory>
#include "core/Ray.h"
#include "Aabb.h"

class Material;

struct Hitrecord;

class Hittable {
public:
     //dmin is minimum acceptable value for ray intersection equation
     //dmax is maximum acceptable value for ray intersection equation
     //record stores intersection details
    virtual bool hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const = 0;
    virtual bool boundingBox(float c0, float c1, Aabb& box) const = 0;
};

struct Hitrecord 
{
    float distance;
    Vector3D p;
    Vector3D normal;
    Material* matptr;
};

#endif //RAYTRACER_HITTABLE_H
