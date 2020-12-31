#include "Camera.h"
#include <cmath>

static const float pi = 3.14159265359f;

Camera::Camera() : aspectratio_(16.f/9.f), origin_({0,0,0}), pointat_({1,0,0}), up_({0,1,0}) 
{
    setFov(90);
    applyChanges();
}

Camera::Camera(float fov, float aspectratio,const Vector3D &origin,const Vector3D &pointat,const Vector3D &up) :
        aspectratio_(aspectratio), origin_(origin), pointat_(pointat), up_(up) 
{
    setFov(fov);
    applyChanges();
}

Ray Camera::getRay(const float &x, const float &y) const 
{
    Vector3D b = lowerleftcorner_ +
                 x * horizontal_ +
                 y * vertical_ - origin_;
    return {origin_, b};
}

void Camera::setFov(const float &fov) 
{
    theta_ = fov * pi / 180; // fov to radians
}

void Camera::setPos(const Vector3D &pos) 
{
    origin_ = pos;
}

void Camera::setUp(const Vector3D &up) 
{
    up_ = up;
}

void Camera::setAspectRatio(const float &ratio) 
{
    aspectratio_ = ratio;
}

void Camera::applyChanges() 
{
    halfwidth_ = tan(theta_ / 2); // tan(theta/2) = (w/2)/(1) = w/2
    halfheight_ = halfwidth_ / aspectratio_; // aspect = w/h => h = w/aspect

    // Orthonormal basis
    Vector3D u, v, w;

    w = origin_ - pointat_;
    w.normalize();

    u = up_.cross(w);
    u.normalize();

    v = w.cross(u);

    lowerleftcorner_ = origin_ - halfwidth_ * u - halfheight_ * v - w;
    horizontal_ = 2 * halfwidth_ * u;
    vertical_ = 2 * halfheight_ * v;

}