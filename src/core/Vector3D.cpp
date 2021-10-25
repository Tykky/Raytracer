#include "Vector3D.h"
#include <cmath>
#include <string>

Vector3D::Vector3D() :
        m_x(0), m_y(0), m_z(0) 
        {}

Vector3D::Vector3D(float x, float y, float z) :
        m_x(x), m_y(y), m_z(z) 
        {}

float Vector3D::getX() const { return m_x; }
float Vector3D::getY() const { return m_y; }
float Vector3D::getZ() const { return m_z; }
float Vector3D::getR() const { return m_x; }
float Vector3D::getG() const { return m_y; }
float Vector3D::getB() const { return m_z; }
float Vector3D::length() const { return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z); }
float Vector3D::lengthSquared() const { return m_x * m_x + m_y * m_y + m_z * m_z; }

void Vector3D::normalize() 
{
    float k = 1/std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
    m_x *= k;
    m_y *= k;
    m_z *= k;
}

const Vector3D &Vector3D::operator+() const {return *this;}

Vector3D Vector3D::operator-() const { return {-m_x, -m_y, -m_z}; }

float Vector3D::operator[](int i) const 
{
    if(i == 0) return m_x;
    if(i == 1) return m_y;
    if(i == 2) return m_z;
    throw std::out_of_range("vector3D : index "+std::to_string(i)+" is out of range");
}

float &Vector3D::operator[](int i) 
{
    if(i == 0) return m_x;
    if(i == 1) return m_y;
    if(i == 2) return m_z;
    throw std::out_of_range("vector3D : index "+std::to_string(i)+" is out of range");
}

Vector3D &Vector3D::operator+=(const Vector3D &v) 
{
    m_x += v.getX();
    m_y += v.getY();
    m_z += v.getZ();
    return *this;
}

Vector3D &Vector3D::operator-=(const Vector3D &v) 
{
    m_x -= v.getX();
    m_y -= v.getY();
    m_z -= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator*=(const Vector3D &v) 
{
    m_x *= v.getX();
    m_y *= v.getY();
    m_z *= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator/=(const Vector3D &v) 
{
    m_x /= v.getX();
    m_y /= v.getY();
    m_z /= v.getZ();
    return *this;
}

Vector3D &Vector3D::operator+=(const float k) 
{
    m_x += k;
    m_y += k;
    m_z += k;
    return *this;
}

Vector3D &Vector3D::operator-=(const float k) 
{
    m_x -= k;
    m_y -= k;
    m_z -= k;
    return *this;
}

Vector3D &Vector3D::operator*=(const float k) 
{
    m_x *= k;
    m_y *= k;
    m_z *= k;
    return *this;
}

Vector3D &Vector3D::operator/=(const float k) 
{
    m_x /= k;
    m_y /= k;
    m_z /= k;
    return *this;
}

std::istream &Vector3D::operator>>(std::istream &is) 
{
    is >> m_x >> m_y >> m_z;
    return is;
}

std::ostream &Vector3D::operator<<(std::ostream &os) 
{
    os << m_x << " " << m_y << " " << m_z;
    return os;
}

float Vector3D::dot(const Vector3D &v) const 
{
    return m_x * v.getX() +
           m_y * v.getY() +
           m_z * v.getZ();
}

Vector3D Vector3D::cross(const Vector3D &v) const 
{
    // a x b = (a2b3-a3b2)i+(a3b1-a1b3)j+(a1b2-a2b1)k
    return {m_y * v.getZ() - m_z * v.getY(),
            m_z * v.getX() - m_x * v.getZ(),
            m_x * v.getY() - m_y * v.getX()};
}

Vector3D operator+(const Vector3D &v1, const Vector3D &v2) 
{
    return {v1.getX()+v2.getX(),
            v1.getY()+v2.getY(),
            v1.getZ()+v2.getZ()};
}

Vector3D operator-(const Vector3D &v1, const Vector3D &v2) 
{
    return {v1.getX()-v2.getX(),
            v1.getY()-v2.getY(),
            v1.getZ()-v2.getZ()};
}

Vector3D operator*(const Vector3D &v1, const Vector3D& v2) 
{
    return {v1.getX()*v2.getX(),
            v1.getY()*v2.getY(),
            v1.getZ()*v2.getZ()};
}

Vector3D operator*(const float &c, const Vector3D &v) 
{
    return {c*v.getX(), c*v.getY(), c*v.getZ()};
}

Vector3D operator*(const Vector3D &v, const float &c) 
{
    return {c*v.getX(), c*v.getY(), c*v.getZ()};
}

Vector3D operator/(const Vector3D &v1, const Vector3D &v2) 
{
    return {v1.getX()/v2.getX(),
            v1.getY()/v2.getY(),
            v1.getZ()/v2.getZ()};
}

Vector3D operator/(const Vector3D &v, const float &c) 
{
    return {v.getX()/c, v.getY()/c, v.getZ()/c};
}