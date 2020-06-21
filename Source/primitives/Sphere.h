#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "core/Vector3D.h"
#include "Primitive.h"

/** @brief sphere primitive handles handles the
 *  mathematical representation of a sphere. */
class Sphere : public Primitive {
private:
    const Vector3D center;
    float radius;
    Material *matptr;
public:
    Sphere(const Vector3D &center, float radius, Material *mat);

    /** @brief Computes solution for ray intersection equation */
    bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const override;

    virtual bool boundingBox(float c0, float c1, Aabb &box) const override;
};


#endif //RAYTRACER_SPHERE_H
