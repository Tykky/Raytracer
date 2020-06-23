#include "Aabb.h"

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
