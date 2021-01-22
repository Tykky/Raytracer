#include "Vector3D.h"
#include <cmath>
#include <string>

Vector3D::Vector3D() :
        x_(0), y_(0), z_(0) 
        {}

Vector3D::Vector3D(float x, float y, float z) :
        x_(x), y_(y), z_(z) 
        {}

float Vector3D::getX() const { return x_; }
float Vector3D::getY() const { return y_; }
float Vector3D::getZ() const { return z_; }
float Vector3D::getR() const { return x_; }
float Vector3D::getG() const { return y_; }
float Vector3D::getB() const { return z_; }
float Vector3D::length() const { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }
float Vector3D::lengthSquared() const { return x_ * x_ + y_ * y_ + z_ * z_; }

void Vector3D::normalize() {
    float k = 1/std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    x_ *= k;
    y_ *= k;
    z_ *= k;
}

const Vector3D &Vector3D::operator+() const {return *this;}

Vector3D Vector3D::operator-() const { return {-x_, -y_, -z_}; }

float Vector3D::operator[](int i) const {
    if(i == 0) return x_;
    if(i == 1) return y_;
    if(i == 2) return z_;
    throw std::out_of_range("vector3D : index "+std::to_string(i)+" is out of range");
}

float &Vector3D::operator[](int i) {
    if(i == 0) return x_;
    if(i == 1) return y_;
    if(i == 2) return z_;
    throw std::out_of_range("vector3D : index "+std::to_string(i)+" is out of range");
}

Vector3D &Vector3D::operator+=(const Vector3D &v) {
    x_ += v.getX();
    y_ += v.getY();
    z_ += v.getZ();
    return *this;
}

Vector3D &Vector3D::operator-=(const Vector3D &v) {
    x_ -= v.getX();
    y_ -= v.getY();
    z_ -= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator*=(const Vector3D &v) {
    x_ *= v.getX();
    y_ *= v.getY();
    z_ *= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator/=(const Vector3D &v) {
    x_ /= v.getX();
    y_ /= v.getY();
    z_ /= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator+=(const float k) {
    x_ += k;
    y_ += k;
    z_ += k;
    return *this;
}

Vector3D &Vector3D::operator-=(const float k) {
    x_ -= k;
    y_ -= k;
    z_ -= k;
    return *this;
}

Vector3D &Vector3D::operator*=(const float k) {
    x_ *= k;
    y_ *= k;
    z_ *= k;
    return *this;
}

Vector3D &Vector3D::operator/=(const float k) {
    x_ /= k;
    y_ /= k;
    z_ /= k;
    return *this;
}

std::istream &Vector3D::operator>>(std::istream &is) {
    is >> x_ >> y_ >> z_;
    return is;
}

std::ostream &Vector3D::operator<<(std::ostream &os) {
    os << x_ << " " << y_ << " " << z_;
    return os;
}

float Vector3D::dot(const Vector3D &v) const {
    return x_ * v.getX() +
           y_ * v.getY() +
           z_ * v.getZ();
}
Vector3D Vector3D::cross(const Vector3D &v) const {
    // a x b = (a2b3-a3b2)i+(a3b1-a1b3)j+(a1b2-a2b1)k
    return {y_ * v.getZ() - z_ * v.getY(),
            z_ * v.getX() - x_ * v.getZ(),
            x_ * v.getY() - y_ * v.getX()};
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