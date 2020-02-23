#include <cmath>
#include "Sphere.h"

using namespace std;

Sphere::Sphere(const vector3D &center, float radius, Material *mat) :
        center(center), radius(radius), matptr(mat) {
}

bool Sphere::hit(const Ray &r, float cmin, float cmax, hitrecord &record) const {
    vector3D oc = r.getOrigin() - center;
    float a = r.getDirection().dot(r.getDirection());
    float b = oc.dot(r.getDirection());
    float c = oc.dot(oc) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        float solution = (-b - sqrt(discriminant)) / a;
        if (solution < cmax && solution > cmin) {
            record.c = solution;
            record.p = r.pointAtC(record.c);
            record.normal = (record.p - center) / radius;
            record.matptr = matptr;
            return true;
        }

        solution = (-b + sqrt(discriminant)) / a;
        if (solution < cmax && solution > cmin) {
            record.c = solution;
            record.p = r.pointAtC(record.c);
            record.normal = (record.p - center) / radius;
            record.matptr = matptr;
            return true;
        }
    }
    return false;
}
