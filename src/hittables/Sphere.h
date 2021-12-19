#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "core/Vector3D.h"
#include "Primitive.h"

class Sphere : public Primitive
{
public:
    Sphere(const Vector3D& center, float radius, Material* material);
    virtual bool hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const override;
    virtual bool boundingBox(float c0, float c1, Aabb& box) const override;

private:
    const Vector3D m_center;
    float m_radius;
    Material* m_matptr;
};


#endif //RAYTRACER_SPHERE_H
