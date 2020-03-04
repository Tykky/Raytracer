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

