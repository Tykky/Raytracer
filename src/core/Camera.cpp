#include "Camera.h"
#include <cmath>

static const float pi = 3.1416;

RTCamera::RTCamera() : m_aspectRatio(16.f/9.f), m_origin({0,0,0}), m_pointat({1,0,0}), m_up({0,1,0}) 
{
    setFov(90);
    applyChanges();
}

RTCamera::RTCamera(float fov, float aspectratio,const Vector3D& origin, const Vector3D& pointat,const Vector3D& up) :
        m_aspectRatio(aspectratio), m_origin(origin), m_pointat(pointat), m_up(up) {
    setFov(fov);
    applyChanges();
}

Ray RTCamera::getRay(float x, float y) const
{
    Vector3D b = m_lowerLeftCorner +
                 x * m_horizontal +
                 y * m_vertical - m_origin;
    return {m_origin, b};
}

void RTCamera::setFov(const float& fov) 
{
    m_theta = fov * pi / 180; // fov to radians
}

void RTCamera::setPos(const Vector3D& pos) 
{
    m_origin = pos;
}

void RTCamera::setUp(const Vector3D& up) 
{
    m_up = up;
}

void RTCamera::setAspectRatio(const float& ratio) 
{
    m_aspectRatio = ratio;
}

void RTCamera::applyChanges() 
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

Ray RTCamera::getRayScreenCoords(int x, int y, unsigned int width, unsigned int height) const
{
    return getRay(static_cast<float>(x) / static_cast<float>(width),
                  static_cast<float>(y) / static_cast<float>(height));
}

Ray RTCamera::getRayScreenCoordsWithJitter(int x, int y, unsigned int width, unsigned int height,
                                         std::function<float()>& randomFloat) const
{    return getRay((static_cast<float>(x) + randomFloat()) / static_cast<float>(width),
                   (static_cast<float>(y) + randomFloat()) / static_cast<float>(height));
}