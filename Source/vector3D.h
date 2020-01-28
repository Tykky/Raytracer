/**@brief vector3D class containing data structure for 3D-euclidean and RGB vectors.
 * @details class contains data structure for 3-dimensional
 * euclidean coordinate vector and 3-dimensional RGB vector.
 */

#ifndef RAYTRACER_VECTOR3D_H
#define RAYTRACER_VECTOR3D_H

#include <iostream>
#include <memory>

class vector3D {

    private:
    float x, y, z;

    public:
    /** Default constructor. Initializes everything to 0. */
    vector3D();
    /** Constructor with parameters. Initializes to parameter values. */
    vector3D(float x, float y, float z);

    /**@brief Computes vector length with pythagoras sqrt(x^2+y^2+z^2) */
    float length() const;
    float lengthSquared() const;

    /**@brief Transforms vector to length 1 vector. */
    void normalize();

    /**@brief Computes vector dot product */
    float dot(const vector3D &v) const;

    /**@brief Computes vector cross product */
    vector3D cross(const vector3D &v) const;

    float getX() const;
    float getY() const;
    float getZ() const;
    float getR() const;
    float getG() const;
    float getB() const;

    const vector3D &operator+ () const;
    vector3D operator- () const;
    float operator[] (int i) const;
    float &operator[] (int i);

    vector3D &operator+= (const vector3D &v);
    vector3D &operator-= (const vector3D &v);
    vector3D &operator*= (const vector3D &v);
    vector3D &operator/= (const vector3D &v);
    vector3D &operator+= (float k);
    vector3D &operator-= (float k);
    vector3D &operator*= (float k);
    vector3D &operator/= (float k);

    std::istream &operator>>(std::istream &is);
    std::ostream &operator<<(std::ostream &os);

};


#endif //RAYTRACER_VECTOR3D_H
