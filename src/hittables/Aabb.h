#ifndef RAYTRACER_AABB_H
#define RAYTRACER_AABB_H

#include "core/Vector3D.h"
#include "core/Ray.h"

// Axis aligned bounding box
class Aabb 
{
public:
    Aabb();
    Aabb(const Vector3D& a, const Vector3D& b);
    Vector3D getMin();
    Vector3D getMax();
    bool hit(const Ray &r, float dmin, float dmax) const;
private:
    Vector3D min_;
    Vector3D max_;
};

Aabb surroundingBox(Aabb box1, Aabb box2);

#endif //RAYTRACER_AABB_H
