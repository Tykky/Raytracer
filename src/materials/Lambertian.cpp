#include "Lambertian.h"
#include "core/utility.h"

Lambertian::Lambertian(const Vector3D &albedo) :
        albedo_(albedo) 
{}

bool Lambertian::scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const 
{
    Vector3D rpoint = record.p + record.normal + randomInUnitSphere(randomFloat);
    scatter = Ray(record.p, rpoint - record.p);
    attenuation = albedo_;
    return true;
}
