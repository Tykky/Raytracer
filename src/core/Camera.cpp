#include "Camera.h"
#include <cmath>

static const float pi = 3.14159265359f;

Camera::Camera() : m_aspectRatio(16.f/9.f), m_origin({0,0,0}), m_pointat({1,0,0}), m_up({0,1,0}) 
{
    setFov(90);
    applyChanges();
}

Camera::Camera(float fov, float aspectratio,const Vector3D& origin, const Vector3D& pointat,const Vector3D& up) :
        m_aspectRatio(aspectratio), m_origin(origin), m_pointat(pointat), m_up(up) {
    setFov(fov);
    applyChanges();
}

Ray Camera::getRay(const float& x, const float& y) const 
{
    Vector3D b = m_lowerLeftCorner +
                 x * m_horizontal +
                 y * m_vertical - m_origin;
    return {m_origin, b};
}

void Camera::setFov(const float& fov) 
{
    m_theta = fov * pi / 180; // fov to radians
}

void Camera::setPos(const Vector3D& pos) 
{
    m_origin = pos;
}

void Camera::setUp(const Vector3D& up) 
{
    m_up = up;
}

void Camera::setAspectRatio(const float& ratio) 
{
    m_aspectRatio = ratio;
}

void Camera::applyChanges() 
{
    m_halfWidth = tan(m_theta / 2); // tan(theta/2) = (w/2)/(1) = w/2
    m_halfHeight = m_halfWidth / m_aspectRatio; // aspect = w/h => h = w/aspect

    // Orthonormal basis
    Vector3D u, v, w;

    w = m_origin - m_pointat;
    w.normalize();

    u = m_up.cross(w);
    u.normalize();

    v = w.cross(u);

    m_lowerLeftCorner = m_origin - m_halfWidth * u - m_halfHeight * v - w;
    m_horizontal = 2 * m_halfWidth * u;
    m_vertical = 2 * m_halfHeight * v;
}