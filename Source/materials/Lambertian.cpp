#include "Lambertian.h"
#include "Utility.h"

Lambertian::Lambertian(const vector3D &albedo) :
        albedo(albedo) {
}

bool Lambertian::scatter(const Ray &r, const hitrecord &record, vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const {
    vector3D rpoint = record.p + record.normal + randomInUnitSphere(randomFloat);
    scatter = Ray(record.p, rpoint - record.p);
    attenuation = albedo;
    return true;
}
