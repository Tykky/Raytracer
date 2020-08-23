#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include <memory>
#include "core/Ray.h"
#include "Aabb.h"

// materials/Material.h already includes this header.
// Including Material.h would cause circular dependency
class Material;

/** @brief Stores information about Ray - Hittable intersection. */
struct Hitrecord;

/**
 * @brief Hittable is abstract class which provides interface for all hittables
 * (such as spheres and triangles)
 */
class Hittable {
public:

    /**
     * @brief Solves hit equation.
     * @param [in] dmin is minimum acceptable value for ray intersection equation
     * @param [in] dmax is maximum acceptable value for ray intersection equation
     * @param [out] record stores intersection details
     * @return true when ray intersection equation has solution between dmin and dmax
     * false.
     */
    virtual bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const = 0;

    /**
     * @param [in] c0
     * @param [in] c1
     * @param [out] box
     * @return when forming bounding box is possible
     */
    virtual bool boundingBox(float c0, float c1, Aabb &box) const = 0;

};

struct Hitrecord {

    /** @brief Solution to hit equation. */
    float distance;

    /** @brief Point where the ray hit. */
    Vector3D p;

    /** @brief Surface normal for point p.
     *  Length of normal vector is always 1. */
    Vector3D normal;

    /** @brief Pointer to material. */
    std::shared_ptr<Material> matptr;
};

#endif //RAYTRACER_HITTABLE_H
