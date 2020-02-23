#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "vector3D.h"
#include "Primitive.h"

/** @brief sphere primitive handles handles the
 *  mathematical representation of a sphere. */
class Sphere : public Primitive {
private:
    const vector3D center;
    float radius;
    Material *matptr;
public:
    Sphere(const vector3D &center, float radius, Material *mat);

    /** @brief Computes solution for Ray - Sphere hit equation
     *  @details hit equation for sphere: dot((Ray(c) - center),(Ray(c) - center))=R^2 (R = radius)
     *  solve for C. Ray function is Ray(c) = origin + c*direction, origin,direction are R^3 vectors. */
    bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const override;
};


#endif //RAYTRACER_SPHERE_H
