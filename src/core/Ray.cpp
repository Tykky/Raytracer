#include "Ray.h"

Ray::Ray() :
        position_({}), direction_({}) 
{};

Ray::Ray(const Vector3D& origin, const Vector3D& direction) :
        position_(origin), direction_(direction) 
{};

Vector3D Ray::getPosition() const { return position_; }
Vector3D Ray::getDirection() const { return direction_; }
Vector3D Ray::pointAtDistance(const float &distance) const { return position_ + distance * direction_; }
