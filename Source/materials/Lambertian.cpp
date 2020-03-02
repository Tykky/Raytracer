#include "Lambertian.h"
#include "Utility.h"

Lambertian::Lambertian(const Vector3D &albedo) :
        albedo(albedo) {
}

bool Lambertian::scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const {
    Vector3D rpoint = record.p + record.normal + randomInUnitSphere(randomFloat);
    scatter = Ray(record.p, rpoint - record.p);
    attenuation = albedo;
    return true;
}
