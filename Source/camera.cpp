#include "camera.h"
#include <cmath>

using namespace std;

camera::camera(float fov, float aspectratio) :
    fov(fov), aspectratio(aspectratio) {
    theta = fov*float(M_PI)/180; // fov to radians
    halfwidth = tan(theta/2); // tan(theta/2) = (w/2)/(1) = w/2
    halfheight = halfwidth/aspectratio; // aspect = w/h => h = w/aspect

    lowerleftcorner = vector3D(-halfwidth,-halfheight,-1);
    horizontal = vector3D(2*halfwidth,0,0);
    vertical = vector3D(0,2*halfheight,0);

}
ray camera::getRay(float x, float y) const {
    vector3D b = lowerleftcorner +
                 x*horizontal +
                 y*vertical;
    // returns (0,0,0) -> b
    return {vector3D(),b};
}
