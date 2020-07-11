#include "Ray.h"

Ray::Ray() :
    position({}), direction({}) {
};

Ray::Ray(const Vector3D& origin, const Vector3D& direction) :
    position(origin), direction(direction) {
};

Vector3D Ray::getPosition() const {
    return position;
}

Vector3D Ray::getDirection() const {
    return direction;
}

Vector3D Ray::pointAtC(float c) const {
    return position + c * direction;
}
