#include "metal.h"

metal::metal(const vector3D &albedo) :
    albedo(albedo) {
}

bool metal::scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                    std::function<float()> &randomFloat) const {
    vector3D v = r.getB();
    v.normalize();
    vector3D reflection = reflect(v,record.normal);
    scatter = ray(record.p,reflection);
    attenuation = albedo;
    return ((scatter.getB().dot(record.normal)) > 0);
}

vector3D reflect(const vector3D &v, const vector3D &n) {
    return v - 2*v.dot(n)*n;
}