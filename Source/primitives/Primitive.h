#ifndef RAYTRACER_PRIMITIVE_H
#define RAYTRACER_PRIMITIVE_H

#include "core/Ray.h"
#include "Aabb.h"

class Material;

/** @brief Stores information about Ray - Primitive intersection. */
struct hitrecord {

    /** @brief Solution to hit equation. */
    float c;

    /** @brief Point where the ray hit. */
    Vector3D p;

    /** @brief Surface normal for point p.
     *  Length of normal vector is always 1. */
    Vector3D normal;

    /** @brief Pointer to material. */
    Material *matptr;
};

/** @brief Primitive is abstract class which provides interface for all primitives
 *  (such as spheres and triangles) */
class Primitive {
public:

    /**
     * @brief Solves hit equation.
     * @param cmin is minimum acceptable value for hit equation's solution.
     * @param cmax is maximum acceptable value for hit equation's solution.
     * @param record is SET by this member function. Empty hitrecord
     * should be given as reference and used after function has ran.
     * @return true when hit equation has solution between cmin and cmax, otherwise
     * false.
     */
    virtual bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const = 0;

    virtual bool boundingBox(float c0, float c1, Aabb &box) const = 0;

};


#endif //RAYTRACER_PRIMITIVE_H
