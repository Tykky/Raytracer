#include "Ray.h"

Ray::Ray() {
    origin = vector3D(0, 0, 0);
    direction = vector3D(0, 0, 0);
}

Ray::Ray(const vector3D &v1, const vector3D &v2) {
    origin = v1;
    direction = v2;
}

vector3D Ray::getOrigin() const {
    return origin;
}

vector3D Ray::getDirection() const {
    return direction;
}

vector3D Ray::pointAtC(float c) const {
    return origin + c * direction;
}
