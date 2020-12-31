#ifndef RAYTRACER_AABB_H
#define RAYTRACER_AABB_H

#include "core/Vector3D.h"
#include "core/Ray.h"

/**
 *  @brief Aabb stands for axis-aligned bounding box.
 *  This defines the bounding box used by the bvh
 */
class Aabb 
{
public:
    Aabb();
    /**
     *  @param a defines three planes where each (x,y,z) is constant
     *  @param b defines three planes where each (x,y,z) is constant
     */
    Aabb(const Vector3D &a, const Vector3D &b);
    Vector3D getMin();
    Vector3D getMax();
    bool hit(const Ray &r, float dmin, float dmax) const;
private:
    Vector3D min_;
    Vector3D max_;
};

/** @brief combines two bounding boxes in one box */
Aabb surroundingBox(Aabb box1, Aabb box2);


#endif //RAYTRACER_AABB_H
