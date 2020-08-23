#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Vector3D.h"

/**
 *  @brief Ray class defines ray
 *  with two vectors. One for position
 *  and one for direction. 
 */
class Ray {

public:
    Ray();
    /**
     * @param [in] position
     * @param [in] direction
     */
    Ray(const Vector3D &position, const Vector3D &direction);
    Vector3D getPosition() const;
    Vector3D getDirection() const;

    /**
     * @param [in] c
     * @return coordinates for scalar multiplication origin + distance * direction
     */
    Vector3D pointAtDistance(float distance) const;

private:
    Vector3D position;
    Vector3D direction;


};

#endif //RAYTRACER_RAY_H
