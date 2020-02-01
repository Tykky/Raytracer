#include "ray.h"

ray::ray() {
    A = vector3D(0,0,0);
    B = vector3D(0,0,0);
}

ray::ray(const vector3D &v1, const vector3D &v2) {
    A = v1;
    B = v2;
}

vector3D ray::getA() const {
    return vector3D();
}

vector3D ray::getB() const {
    return vector3D();
}

vector3D ray::scalarMult(const float c) const {
    return A + c*B;
}
