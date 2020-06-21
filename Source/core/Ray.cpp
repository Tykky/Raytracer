#include "Ray.h"

Ray::Ray() {
    origin = Vector3D(0, 0, 0);
    direction = Vector3D(0, 0, 0);
}

Ray::Ray(const Vector3D &v1, const Vector3D &v2) {
    origin = v1;
    direction = v2;
}

Vector3D Ray::getOrigin() const {
    return origin;
}

Vector3D Ray::getDirection() const {
    return direction;
}

Vector3D Ray::pointAtC(float c) const {
    return origin + c * direction;
}
