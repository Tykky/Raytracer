#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include <memory>
#include "core/Ray.h"
#include "Aabb.h"

// materials/Material.h already includes this header.
// Including Material.h would cause circular dependency
class Material;

struct Hitrecord;

/**
 * @brief Hittable is abstract class which provides interface for all hittables
 * (such as spheres and triangles)
 */
class Hittable {
public:

    /**
     * @brief Solves hit equation.
     * @param cmin is minimum acceptable value for hit equations solution.
     * @param cmax is maximum acceptable value for hit equations solution.
     * @param record is SET by this member function. Empty hitrecord
     * should be given as reference and used after function has ran.
     * @return true when hit equation has solution between cmin and cmax, otherwise
     * false.
     */
    virtual bool hit(const Ray &r, float cmin, float cmax, Hitrecord &record) const = 0;

    virtual bool boundingBox(float c0, float c1, Aabb &box) const = 0;

};

/** @brief Stores information about Ray - Hittable intersection. */
struct Hitrecord {

    /** @brief Solution to hit equation. */
    float c;

    /** @brief Point where the ray hit. */
    Vector3D p;

    /** @brief Surface normal for point p.
     *  Length of normal vector is always 1. */
    Vector3D normal;

    /** @brief Pointer to material. */
    std::shared_ptr<Material> matptr;
};

#endif //RAYTRACER_HITTABLE_H
