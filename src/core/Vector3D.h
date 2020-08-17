#ifndef RAYTRACER_VECTOR3D_H
#define RAYTRACER_VECTOR3D_H

#include <iostream>

class Vector3D {

public:
    Vector3D();
    Vector3D(float x, float y, float z);

    float length() const;
    float lengthSquared() const;

    void normalize();

    float dot(const Vector3D &v) const;

    Vector3D cross(const Vector3D &v) const;

    float getX() const;
    float getY() const;
    float getZ() const;
    float getR() const;
    float getG() const;
    float getB() const;

    const Vector3D &operator+ () const;
    Vector3D operator- () const;
    float operator[] (int i) const;
    float &operator[] (int i);

    Vector3D &operator+= (const Vector3D &v);
    Vector3D &operator-= (const Vector3D &v);
    Vector3D &operator*= (const Vector3D &v);
    Vector3D &operator/= (const Vector3D &v);
    Vector3D &operator+= (float k);
    Vector3D &operator-= (float k);
    Vector3D &operator*= (float k);
    Vector3D &operator/= (float k);

    std::istream &operator>>(std::istream &is);
    std::ostream &operator<<(std::ostream &os);

private:
    float x, y, z;

};

Vector3D operator+(const Vector3D &v1, const Vector3D &v2);
Vector3D operator-(const Vector3D &v1, const Vector3D &v2);
Vector3D operator*(const Vector3D &v1, const Vector3D& v2);
Vector3D operator*(const float &c, const Vector3D &v);
Vector3D operator*(const Vector3D &v, const float &c);
Vector3D operator/(const Vector3D &v1, const Vector3D &v2);
Vector3D operator/(const Vector3D &v, const float &c);


#endif //RAYTRACER_VECTOR3D_H