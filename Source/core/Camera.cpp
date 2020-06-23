#include "Camera.h"
#include <cmath>

Camera::Camera(float fov, float aspectratio, Vector3D origin, Vector3D pointat, Vector3D up) :
        aspectratio(aspectratio), origin(origin) {

    const float pi = 3.14159265359;
	
    theta = fov * pi / 180; // fov to radians
    halfwidth = tan(theta / 2); // tan(theta/2) = (w/2)/(1) = w/2
    halfheight = halfwidth / aspectratio; // aspect = w/h => h = w/aspect

    // Orthonormal basis
    Vector3D u, v, w;

    w = origin - pointat;
    w.normalize();

    u = up.cross(w);
    u.normalize();

    v = w.cross(u);

    lowerleftcorner = origin - halfwidth * u - halfheight * v - w;
    horizontal = 2 * halfwidth * u;
    vertical = 2 * halfheight * v;

}

Ray Camera::getRay(float x, float y) const {
    Vector3D b = lowerleftcorner +
                 x * horizontal +
                 y * vertical - origin;
    return {origin, b};
}
