#include <cmath>
#include "sphere.h"

using namespace std;

sphere::sphere(const vector3D &center, float radius) :
    center(center), radius(radius) {
}

bool sphere::hit(const ray &r, float cmin, float cmax, hitrecord &record) const {
    vector3D oc = r.getA() - center;
    float a = r.getB().dot(r.getB());
    float b = oc.dot(r.getB());
    float c = oc.dot(oc) -radius*radius;
    float discriminant = b*b - a*c;

    if(discriminant > 0 ) {
        float solution = (-b - sqrt(discriminant))/a;
        if(solution < cmax && solution > cmin) {
            record.c = solution;
            record.p = r.pointAtC(record.c);
            record.normal = (record.p - center);
            return true;
        }

        solution = (-b + sqrt(discriminant))/a;
        if(solution < cmax && solution > cmin) {
            record.c = solution;
            record.p = r.pointAtC(record.c);
            record.normal = (record.p - center)/radius;
            return true;
        }
    }
    return false;
}
