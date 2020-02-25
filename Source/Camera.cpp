#include "Camera.h"
#include <cmath>

using namespace std;

Camera::Camera(float fov, float aspectratio, vector3D origin, vector3D pointat, vector3D up) :
        fov(fov), aspectratio(aspectratio), origin(origin) {

    theta = fov * float(M_PI) / 180; // fov to radians
    halfwidth = tan(theta / 2); // tan(theta/2) = (w/2)/(1) = w/2
    halfheight = halfwidth / aspectratio; // aspect = w/h => h = w/aspect

    // Orthonormal basis
    vector3D u, v, w;

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
    vector3D b = lowerleftcorner +
                 x * horizontal +
                 y * vertical - origin;
    // returns origin -> (x,y)
    return {origin, b};
}
