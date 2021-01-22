#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "core/Vector3D.h"
#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const Vector3D &center, float radius, Material *material);
    bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const override;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const override;

private:
    const Vector3D center_;
    float radius_;
    Material *matptr_;
};


#endif //RAYTRACER_SPHERE_H
