/**@brief Vector3d class containing data structure for 3D-euclidean and 3D-RGB vectors.
 * @details class contains data structure for 3-dimensional
 * euclidean coordinate vector and 3-dimensional RGB vector.
 */

#ifndef RAYTRACER_VECTOR3D_H
#define RAYTRACER_VECTOR3D_H

#include <iostream>

class Vector3d {

    private:
    float x, y, z;

    public:
    /** Default constructor. Initialized everything to 0. */
    Vector3d();
    /** Constructor with parameters. Initializes to parameter values. */
    Vector3d(float x, float y, float z);

    /**@brief Computes vector length with pythagoras sqrt(x^2+y^2+z^2) */
    float length() const;
    float lengthSquared() const;

    /**@brief Transforms vector to length 1 vector. */
    void makeUnitVector();

    float getX() const;
    float getY() const;
    float getZ() const;
    float getR() const;
    float getG() const;
    float getB() const;

    const Vector3d &operator+ () const;
    Vector3d operator- () const;
    float operator[] (int i) const;
    float &operator[] (int i);

    Vector3d &operator+= (const Vector3d &v);
    Vector3d &operator-= (const Vector3d &v);
    Vector3d &operator*= (const Vector3d &v);
    Vector3d &operator/= (const Vector3d &v);
    Vector3d &operator+= (const float k);
    Vector3d &operator-= (const float k);
    Vector3d &operator*= (const float k);
    Vector3d &operator/= (const float k);

    std::istream &operator>>(std::istream &is);
    std::ostream &operator<<(std::ostream &os);

};


#endif //RAYTRACER_VECTOR3D_H
