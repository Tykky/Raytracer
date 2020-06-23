#include "Ray.h"

Ray::Ray() :
    origin({}), direction({}) {
};

Ray::Ray(const Vector3D& origin, const Vector3D& direction) :
    origin(origin), direction(direction) {
};

Vector3D Ray::getOrigin() const {
    return origin;
}

Vector3D Ray::getDirection() const {
    return direction;
}

Vector3D Ray::pointAtC(float c) const {
    return origin + c * direction;
}
