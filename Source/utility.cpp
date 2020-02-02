#include "utility.h"

vector3D skyGradient(const ray &r) {
    vector3D direction = r.getB();
    direction.normalize();
    float t = 0.5*(direction.getY()+1);
    return (1.0-t)*vector3D(1.0, 1.0, 1.0) + t*vector3D(0.5, 0.7, 1.0);
}
