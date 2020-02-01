#include "vector3D.h"
#include <cmath>

vector3D::vector3D() :
    x(0), y(0), z(0) {
}

vector3D::vector3D(float x, float y, float z) :
    x(x), y(y), z(z) {
}

float vector3D::getX() const {
    return x;
}

float vector3D::getY() const {
    return y;
}

float vector3D::getZ() const {
    return z;
}

float vector3D::getR() const {
    return x;
}

float vector3D::getG() const {
    return y;
}

float vector3D::getB() const {
    return z;
}

float vector3D::length() const {
    return std::sqrt(x*x + y*y + z*z);
}

float vector3D::lengthSquared() const {
    return x*x + y*y + z*z;
}

void vector3D::normalize() {
    float k = 1/std::sqrt(x*x + y*y + z*z);
    x *= k;
    y *= k;
    z *= k;
}

const vector3D &vector3D::operator+() const {
    return *this;
}

vector3D vector3D::operator-() const {
    return {-x,-y,-z};
}

float vector3D::operator[](int i) const {
    if(i == 0) return x;
    if(i == 1) return y;
    if(i == 2) return z;
    throw std::out_of_range("vector3D : index "+std::to_string(i)+" is out of range");
}

float &vector3D::operator[](int i) {
    if(i == 0) return x;
    if(i == 1) return y;
    if(i == 2) return z;
    throw std::out_of_range("vector3D : index "+std::to_string(i)+" is out of range");
}

vector3D &vector3D::operator+=(const vector3D &v) {
    x += v.getX();
    y += v.getY();
    z += v.getZ();
    return *this;
}

vector3D &vector3D::operator-=(const vector3D &v) {
    x -= v.getX();
    y -= v.getY();
    z -= v.getZ();
    return *this;
}

vector3D &vector3D::operator*=(const vector3D &v) {
    x *= v.getX();
    y *= v.getY();
    z *= v.getZ();
    return *this;
}

vector3D &vector3D::operator/=(const vector3D &v) {
    x /= v.getX();
    y /= v.getY();
    z /= v.getZ();
    return *this;
}

vector3D &vector3D::operator+=(const float k) {
    x += k;
    y += k;
    z += k;
    return *this;
}

vector3D &vector3D::operator-=(const float k) {
    x -= k;
    y -= k;
    z -= k;
    return *this;
}

vector3D &vector3D::operator*=(const float k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
}

vector3D &vector3D::operator/=(const float k) {
    x /= k;
    y /= k;
    z /= k;
    return *this;
}

std::istream &vector3D::operator>>(std::istream &is) {
    is >> x >> y >> z;
    return is;
}

std::ostream &vector3D::operator<<(std::ostream &os) {
    os << x << " " << y << " " << z;
    return os;
}

float vector3D::dot(const vector3D &v) const {
    return x*v.getX()+
           y*v.getY()+
           z*v.getZ();
}
vector3D vector3D::cross(const vector3D &v) const {
    // a x b = (a2b3-a3b2)i+(a3b1-a1b3)j+(a1b2-a2b1)k
    return {y * v.getZ() - z * v.getY(),
            z * v.getX() - x * v.getZ(),
            x * v.getY() - y * v.getX()};
}

vector3D operator+(const vector3D &v1, const vector3D &v2) {
    return {v1.getX()+v2.getX(),
            v1.getY()+v2.getY(),
            v1.getZ()+v2.getZ()};
}
vector3D operator-(const vector3D &v1, const vector3D &v2) {
    return {v1.getX()-v2.getX(),
            v1.getY()-v2.getY(),
            v1.getZ()-v2.getZ()};
}
vector3D operator*(const vector3D &v1, const vector3D& v2) {
    return {v1.getX()*v2.getX(),
            v1.getY()*v2.getY(),
            v1.getZ()*v2.getZ()};
}

vector3D operator*(const float &c, const vector3D &v) {
    return {c*v.getX(), c*v.getY(), c*v.getZ()};
}

vector3D operator*(const vector3D &v, const float &c) {
    return {c*v.getX(), c*v.getY(), c*v.getZ()};
}

vector3D operator/(const vector3D &v1, const vector3D &v2) {
    return {v1.getX()/v2.getX(),
            v1.getY()/v2.getY(),
            v1.getZ()/v2.getZ()};
}
vector3D operator/(const vector3D &v, const float &c) {
    return {v.getX()/c, v.getY()/c, v.getZ()/c};
}