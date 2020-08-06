#include "Vector3D.h"
#include <cmath>
#include <string>

Vector3D::Vector3D() :
        x(0), y(0), z(0) {
}

Vector3D::Vector3D(float x, float y, float z) :
        x(x), y(y), z(z) {
}

float Vector3D::getX() const {
    return x;
}

float Vector3D::getY() const {
    return y;
}

float Vector3D::getZ() const {
    return z;
}

float Vector3D::getR() const {
    return x;
}

float Vector3D::getG() const {
    return y;
}

float Vector3D::getB() const {
    return z;
}

float Vector3D::length() const {
    return std::sqrt(x*x + y*y + z*z);
}

float Vector3D::lengthSquared() const {
    return x*x + y*y + z*z;
}

void Vector3D::normalize() {
    float k = 1/std::sqrt(x*x + y*y + z*z);
    x *= k;
    y *= k;
    z *= k;
}

const Vector3D &Vector3D::operator+() const {
    return *this;
}

Vector3D Vector3D::operator-() const {
    return {-x,-y,-z};
}

float Vector3D::operator[](int i) const {
    if(i == 0) return x;
    if(i == 1) return y;
    if(i == 2) return z;
    throw std::out_of_range("vector3D : index "+std::to_string(i)+" is out of range");
}

float &Vector3D::operator[](int i) {
    if(i == 0) return x;
    if(i == 1) return y;
    if(i == 2) return z;
    throw std::out_of_range("vector3D : index "+std::to_string(i)+" is out of range");
}

Vector3D &Vector3D::operator+=(const Vector3D &v) {
    x += v.getX();
    y += v.getY();
    z += v.getZ();
    return *this;
}

Vector3D &Vector3D::operator-=(const Vector3D &v) {
    x -= v.getX();
    y -= v.getY();
    z -= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator*=(const Vector3D &v) {
    x *= v.getX();
    y *= v.getY();
    z *= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator/=(const Vector3D &v) {
    x /= v.getX();
    y /= v.getY();
    z /= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator+=(const float k) {
    x += k;
    y += k;
    z += k;
    return *this;
}

Vector3D &Vector3D::operator-=(const float k) {
    x -= k;
    y -= k;
    z -= k;
    return *this;
}

Vector3D &Vector3D::operator*=(const float k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
}

Vector3D &Vector3D::operator/=(const float k) {
    x /= k;
    y /= k;
    z /= k;
    return *this;
}

std::istream &Vector3D::operator>>(std::istream &is) {
    is >> x >> y >> z;
    return is;
}

std::ostream &Vector3D::operator<<(std::ostream &os) {
    os << x << " " << y << " " << z;
    return os;
}

float Vector3D::dot(const Vector3D &v) const {
    return x*v.getX()+
           y*v.getY()+
           z*v.getZ();
}
Vector3D Vector3D::cross(const Vector3D &v) const {
    // a x b = (a2b3-a3b2)i+(a3b1-a1b3)j+(a1b2-a2b1)k
    return {y * v.getZ() - z * v.getY(),
            z * v.getX() - x * v.getZ(),
            x * v.getY() - y * v.getX()};
}

Vector3D operator+(const Vector3D &v1, const Vector3D &v2) {
    return {v1.getX()+v2.getX(),
            v1.getY()+v2.getY(),
            v1.getZ()+v2.getZ()};
}
Vector3D operator-(const Vector3D &v1, const Vector3D &v2) {
    return {v1.getX()-v2.getX(),
            v1.getY()-v2.getY(),
            v1.getZ()-v2.getZ()};
}
Vector3D operator*(const Vector3D &v1, const Vector3D& v2) {
    return {v1.getX()*v2.getX(),
            v1.getY()*v2.getY(),
            v1.getZ()*v2.getZ()};
}

Vector3D operator*(const float &c, const Vector3D &v) {
    return {c*v.getX(), c*v.getY(), c*v.getZ()};
}

Vector3D operator*(const Vector3D &v, const float &c) {
    return {c*v.getX(), c*v.getY(), c*v.getZ()};
}

Vector3D operator/(const Vector3D &v1, const Vector3D &v2) {
    return {v1.getX()/v2.getX(),
            v1.getY()/v2.getY(),
            v1.getZ()/v2.getZ()};
}
Vector3D operator/(const Vector3D &v, const float &c) {
    return {v.getX()/c, v.getY()/c, v.getZ()/c};
}