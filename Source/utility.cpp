#include <primitives/sphere.h>
#include "utility.h"
#include <limits>
#include <random>
#include <functional>
#include <cstdlib>

using namespace std;

const int SCATTERDEPTH = 5;

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

vector3D recursiveScatter(const ray &r, primitive *world, int depth, std::function<float()> &randomFloat) {
    hitrecord record;
    if(world->hit(r,0.001,numeric_limits<float>::max(),record)) {
        ray scatter;
        vector3D attenuation;
        if(depth < SCATTERDEPTH && record.matptr->scatter(r,record,attenuation,scatter,randomFloat)) {
            return attenuation*recursiveScatter(scatter,world,depth+1,randomFloat);
        } else {
            return vector3D();
        }
    }
    return skyGradient(r);
}

vector3D randomInUnitSphere(std::function<float()> &randomFloat) {
    vector3D point;
    while(true) {
        point = 2.0*vector3D(randomFloat(),randomFloat(),randomFloat())-vector3D(1,1,1);
        if(point.lengthSquared() < 1.0) {
            return point;
        }
    }
}

