#include <primitives/sphere.h>
#include "utility.h"
#include <iostream>

using namespace std;

vector3D skyGradient(const ray &r) {
    vector3D direction = r.getB();
    direction.normalize();
    float t = 0.5*(direction.getY()+1);
    return (1.0-t)*vector3D(1.0, 1.0, 1.0) + t*vector3D(0.5, 0.7, 1.0);
}
vector3D sphereNormalColor(hitrecord record) {
    vector3D n = record.normal;
    n.normalize();
    return 0.5*vector3D(n.getX()+1,n.getY()+1,n.getZ()+1);
}