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
    Ray(const Vector3D &origin, const Vector3D &direction);
    Vector3D getPosition() const;
    Vector3D getDirection() const;

    /**
     * @return coordinates for point c
     * along line position -> direction
     */
    Vector3D pointAtC(float c) const;

private:
    Vector3D position;
    Vector3D direction;


};

#endif //RAYTRACER_RAY_H
