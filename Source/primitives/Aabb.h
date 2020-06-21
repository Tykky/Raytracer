/** @file */

#ifndef RAYTRACER_AABB_H
#define RAYTRACER_AABB_H

#include "core/Vector3D.h"
#include "core/Ray.h"

// Simple min & max functions for floats
// fmax and fmin are already reserved.
// These should be faster than standard fmax & fmin.

/** @brief fast version of fmin */
inline float ffmin(float a, float b) {
    return a < b ? a : b;
}
/** @brief fast version of fmax */
inline float ffmax(float a, float b) {
    return a > b ? a : b;
}

/** @brief Aabb stands for axis-aligned bounding box
 * @details This defines the bounding box used by bvh
 * data structure. **/
class Aabb {

private:
    Vector3D min;
    Vector3D max;

public:
    Aabb();
    /** @param a defines three planes x=a[0], y=a[1] and z=a[2]
     *  @param b defines three planes x=b[0], y=b[1] and z=b[2] */
    Aabb(const Vector3D &a, const Vector3D &b);
    Vector3D getMin();
    Vector3D getMax();

    /** @brief Tests if ray hits bounding box
     * @details Method proposed by Andrew Kensler
     * at Pixar. Source: Ray Tracing In The Next Week (Peter Shirley)
     * @return true when ray hits bounding box */
    inline bool hit(const Ray &r, float cmin, float cmax) const {
        for (int a = 0; a < 3; a++) {
            float invD = 1.0f / r.getDirection()[a];
            float t0 = (min[a] - r.getOrigin()[a]) * invD;
            float t1 = (max[a] - r.getOrigin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            cmin = t0 > cmin ? t0 : cmin;
            cmax = t1 < cmax ? t1 : cmax;
            if (cmax <= cmin)
                return false;
        }
        return true;
    }
};


#endif //RAYTRACER_AABB_H
