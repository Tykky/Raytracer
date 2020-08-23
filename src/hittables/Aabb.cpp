#include "Aabb.h"

namespace {

    // Simple min & max functions for floats
    // These should be faster than standard fmax & fmin.

    float ffmin(float a, float b) {
        return a < b ? a : b;
    }

    float ffmax(float a, float b) {
        return a > b ? a : b;
    }

}

Aabb::Aabb(const Vector3D &a, const Vector3D &b) {
    min = a;
    max = b;
}

Vector3D Aabb::getMin() {
    return min;
}

Vector3D Aabb::getMax() {
    return max;
}

bool Aabb::hit(const Ray &r, float dmin, float dmax) const {
    // src: Ray Tracing In The Next Week
    for (int a = 0; a < 3; a++) {
        float invD = 1.0f / r.getDirection()[a];
        float t0 = (min[a] - r.getPosition()[a]) * invD;
        float t1 = (max[a] - r.getPosition()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        dmin = t0 > dmin ? t0 : dmin;
        dmax = t1 < dmax ? t1 : dmax;
        if (dmax <= dmin)
            return false;
    }
    return true;
}

Aabb::Aabb() {}

Aabb surroundingBox(Aabb box1, Aabb box2) {

    Vector3D small(ffmin(box1.getMin().getX(), box2.getMin().getX()),
        ffmin(box1.getMin().getY(), box2.getMin().getX()),
        ffmin(box1.getMin().getZ(), box2.getMin().getZ()));

    Vector3D big(ffmax(box1.getMax().getX(), box2.getMax().getX()),
        ffmax(box1.getMax().getY(), box2.getMax().getY()),
        ffmax(box1.getMax().getZ(), box2.getMax().getZ()));

    return Aabb(small, big);
}
