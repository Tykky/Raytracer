#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "Vector3D.h"
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

    /** @brief Computes solution for Ray - Sphere hit equation
     *  @details hit equation for sphere: \f$ (\text{ray}(c) -
     *  \text{center}) \cdot (\text{ray}(c) - \text{center}) = r^2 \f$
     *  solve for C. Ray function is  \f$ \text{ray}(c) = \text{origin} + c \cdot \text{direction} \f$
     *  where \f$ \text{origin} , \text{direction} \f$ and
     *  \f$ \text{center} \f$ are \f$ \mathbb{R}^3 \f$ vectors and \f$ r \f$ is radius */
    bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const override;

    virtual bool boundingBox(float c0, float c1, Aabb &box) const override;
};


#endif //RAYTRACER_SPHERE_H
