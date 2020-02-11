#include <cmath>
#include "sphere.h"

using namespace std;

sphere::sphere(const vector3D &center, float radius, material *mat) :
    center(center), radius(radius), matptr(mat) {
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
            record.normal.normalize();
            record.matptr = matptr;
            return true;
        }

        solution = (-b + sqrt(discriminant))/a;
        if(solution < cmax && solution > cmin) {
            record.c = solution;
            record.p = r.pointAtC(record.c);
            record.normal = (record.p - center)/radius;
            record.normal.normalize();
            record.matptr = matptr;
            return true;
        }
    }
    return false;
}
