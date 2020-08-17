#ifndef RAYTRACER_AABB_H
#define RAYTRACER_AABB_H

#include "core/Vector3D.h"
#include "core/Ray.h"

/**
 *  @brief Aabb stands for axis-aligned bounding box.
 *  This defines the bounding box used by bvh
 */
class Aabb {

public:
    Aabb();
	
    /**
     *  @param a defines three planes x=a[0], y=a[1] and z=a[2]
     *  @param b defines three planes x=b[0], y=b[1] and z=b[2]
     */
    Aabb(const Vector3D &a, const Vector3D &b);
    Vector3D getMin();
    Vector3D getMax();

    bool hit(const Ray &r, float cmin, float cmax) const;

private:
    Vector3D min;
    Vector3D max;

};

/** @brief combines two bounding boxes in one box */
Aabb surroundingBox(Aabb box1, Aabb box2);


#endif //RAYTRACER_AABB_H
