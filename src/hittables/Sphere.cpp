#include <cmath>
#include "Sphere.h"

Sphere::Sphere(const Vector3D &center, float radius, Material *material) :
        center_(center), radius_(radius), matptr_(material) {
}

bool Sphere::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const {

	Vector3D oc = r.getPosition() - center_;
    float a = r.getDirection().dot(r.getDirection());
    float b = oc.dot(r.getDirection());
    float c = oc.dot(oc) - radius_ * radius_;
    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        const float sqroot = std::sqrt(discriminant);
        float solution = (-b - sqroot) / a;
        if (solution < dmax && solution > dmin) {
            record.distance = solution;
            record.p = r.pointAtDistance(record.distance);
            record.normal = (record.p - center_) / radius_;
            record.matptr = matptr_;
            return true;
        }

        solution = (-b + sqroot) / a;
        if (solution < dmax && solution > dmin) {
            record.distance = solution;
            record.p = r.pointAtDistance(record.distance);
            record.normal = (record.p - center_) / radius_;
            record.matptr = matptr_;
            return true;
        }
    }
    return false;
}

bool Sphere::boundingBox(float c0, float c1, Aabb &box) const {
    box = Aabb(center_ - Vector3D(radius_, radius_, radius_),
               center_ + Vector3D(radius_, radius_, radius_));
    return true;
}
