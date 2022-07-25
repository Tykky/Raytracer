#include "Ray.h"

Ray::Ray() :
        m_position({}), m_direction({}) 
{};

Ray::Ray(const Vector3D& origin, const Vector3D& direction) :
        m_position(origin), m_direction(direction) 
 {};

Vector3D Ray::getPosition() const { return m_position; }
Vector3D Ray::getDirection() const { return m_direction; }
Vector3D Ray::pointAtDistance(const float& distance) const { return m_position + distance * m_direction; }
