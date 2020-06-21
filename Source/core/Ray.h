#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Vector3D.h"

/** @brief Ray class handles mapping origin->direction between two Vector3D vectors. **/
class Ray {

private:
    Vector3D origin;
    Vector3D direction;
public:
    /** @brief Initialize to (0,0,0) */
    Ray();

    /** @brief Initialize to parameter values */
    Ray(const Vector3D &v1, const Vector3D &v2);

    Vector3D getOrigin() const;

    Vector3D getDirection() const;

    /** @return Returns coordinates for point c
     * along line origin -> direction */
    Vector3D pointAtC(float c) const;


};


#endif //RAYTRACER_RAY_H
