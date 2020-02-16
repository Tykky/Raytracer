#include "lambertian.h"
#include "utility.h"

lambertian::lambertian(const vector3D &albedo) :
    albedo(albedo) {
}

bool lambertian::scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                         std::function<float()> &randomFloat) const {
    vector3D rpoint = record.p + record.normal + randomInUnitSphere(randomFloat);
    scatter = ray(record.p, rpoint-record.p);
    attenuation = albedo;
    return true;
}
