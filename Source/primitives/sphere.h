#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "vector3D.h"
#include "primitive.h"

class sphere: public primitive {
private:
    vector3D center;
    float radius;
    material *matptr;
public:
    sphere(const vector3D &center, float radius, material* mat);

    /** @brief Computes solution for hit equation.
     *  @detail hit equation: dot((ray(c)-center),(ray(c)-center))=R^2 (R = radius)
     *  solve for C. Ray function is ray(c)=A+c*B, A,B are R^3 vectors. */
    bool hit(const ray &r, float cmin, float cmax, hitrecord &record) const override;
};


#endif //RAYTRACER_SPHERE_H
