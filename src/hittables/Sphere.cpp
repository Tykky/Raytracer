#include <cmath>
#include "Sphere.h"

Sphere::Sphere(const Vector3D &center, float radius, std::shared_ptr<Material> material) :
        center(center), radius(radius), matptr(material) {
}

bool Sphere::hit(const Ray &r, float cmin, float cmax, Hitrecord &record) const {

	Vector3D oc = r.getPosition() - center;
    float a = r.getDirection().dot(r.getDirection());
    float b = oc.dot(r.getDirection());
    float c = oc.dot(oc) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        const float sqroot = std::sqrt(discriminant);
        float solution = (-b - sqroot) / a;
        if (solution < cmax && solution > cmin) {
            record.c = solution;
            record.p = r.pointAtC(record.c);
            record.normal = (record.p - center) / radius;
            record.matptr = matptr;
            return true;
        }

        solution = (-b + sqroot) / a;
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

bool Sphere::boundingBox(float c0, float c1, Aabb &box) const {
    box = Aabb(center - Vector3D(radius,radius,radius),
               center + Vector3D(radius,radius,radius));
    return true;
}