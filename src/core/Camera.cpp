#include "Camera.h"
#include <cmath>

Camera::Camera(float fov, float aspectratio,const Vector3D &origin,const Vector3D &pointat,const Vector3D &up) :
        aspectratio_(aspectratio), origin_(origin) {

    const float pi = 3.14159265359f;

    theta_ = fov * pi / 180; // fov to radians
    halfwidth_ = tan(theta_ / 2); // tan(theta/2) = (w/2)/(1) = w/2
    halfheight_ = halfwidth_ / aspectratio; // aspect = w/h => h = w/aspect

    // Orthonormal basis
    Vector3D u, v, w;

    w = origin - pointat;
    w.normalize();

    u = up.cross(w);
    u.normalize();

    v = w.cross(u);

    lowerleftcorner_ = origin - halfwidth_ * u - halfheight_ * v - w;
    horizontal_ = 2 * halfwidth_ * u;
    vertical_ = 2 * halfheight_ * v;

}

Ray Camera::getRay(const float &x, const float &y) const {
    Vector3D b = lowerleftcorner_ +
                 x * horizontal_ +
                 y * vertical_ - origin_;
    return {origin_, b};
}
