#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Vector3D.h"

class Ray 
{
public:
    Ray();
    Ray(const Vector3D& position, const Vector3D& direction);
    Vector3D getPosition() const;
    Vector3D getDirection() const;
    Vector3D pointAtDistance(const float& distance) const;

private:
    Vector3D m_position;
    Vector3D m_direction;

};

#endif //RAYTRACER_RAY_H
