/** @brief ray class handles mapping A->B between two vector3D vectors. **/

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include "vector3D.h"

class ray {

private:
    vector3D A;
    vector3D B;
public:
    /** Initialize to (0,0,0) */
    ray();

    /** Initialize to parameter values */
    ray(const vector3D &v1, const vector3D &v2);

    vector3D getA() const;
    vector3D getB() const;

    /** Scalar multiplication along A->B. */
    vector3D scalarMult(float c) const;

};


#endif //RAYTRACER_RAY_H
