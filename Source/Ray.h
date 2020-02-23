
#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vector3D.h"

/** @brief Ray class handles mapping origin->direction between two vector3D vectors. **/
class Ray {

private:
    vector3D origin;
    vector3D direction;
public:
    /** @brief Initialize to (0,0,0) */
    Ray();

    /** @brief Initialize to parameter values */
    Ray(const vector3D &v1, const vector3D &v2);

    vector3D getOrigin() const;

    vector3D getDirection() const;

    /** @return Returns coordinates for point c
     * along line origin -> direction */
    vector3D pointAtC(float c) const;


};


#endif //RAYTRACER_RAY_H
