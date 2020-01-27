#include "Vector3d.h"
#include <cmath>


Vector3d::Vector3d() :
    x(0), y(0), z(0) {
}

Vector3d::Vector3d(float x, float y, float z) :
    x(x), y(y), z(y) {
}

float Vector3d::getX() const {
    return x;
}

float Vector3d::getY() const {
    return y;
}

float Vector3d::getZ() const {
    return z;
}

float Vector3d::getR() const {
    return x;
}

float Vector3d::getG() const {
    return y;
}

float Vector3d::getB() const {
    return z;
}

float Vector3d::length() const {
    return std::sqrt(x*x + y*y + z*z);
}

float Vector3d::lengthSquared() const {
    return x*x + y*y + z*z;
}

void Vector3d::makeUnitVector() {

}

const Vector3d &Vector3d::operator+() const {
    return *this;
}

Vector3d Vector3d::operator-() const {
    return {-x,-y,-z};
}

float Vector3d::operator[](int i) const {
}

float &Vector3d::operator[](int i) {
}

Vector3d &Vector3d::operator+=(const Vector3d &v) {
}

Vector3d &Vector3d::operator-=(const Vector3d &v) {
}

Vector3d &Vector3d::operator*=(const Vector3d &v) {
}

Vector3d &Vector3d::operator/=(const Vector3d &v) {
}

Vector3d &Vector3d::operator+=(const float k) {
}

Vector3d &Vector3d::operator-=(const float k) {
}

Vector3d &Vector3d::operator*=(const float k) {
}

Vector3d &Vector3d::operator/=(const float k) {
}

std::istream &Vector3d::operator>>(std::istream &is) {
    is >> x >> y >> z;
    return is;
}

std::ostream &Vector3d::operator<<(std::ostream &os) {
    os << x << y << z;
    return os;
}
