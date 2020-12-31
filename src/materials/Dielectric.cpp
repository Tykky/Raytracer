#include "Dielectric.h"
#include "core/utility.h"

Dielectric::Dielectric(float ior) :
    ior_(ior) 
{}

bool Dielectric::scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const 
{
    Vector3D outnormal;
    Vector3D reflection = reflect(r.getDirection(), record.normal);
    Vector3D refraction;

    float tmpior;

    // No attenuation
    attenuation = Vector3D(1.0, 1.0, 1.0);

    float probability;
    float cosine;

    if (r.getDirection().dot(record.normal) > 0)
    {
        outnormal = -record.normal;
        tmpior = ior_;
        cosine = ior_ * r.getDirection().dot(record.normal) / r.getDirection().length();
    } else 
    {
        outnormal = record.normal;
        tmpior = 1.0 / ior_;
        cosine = -r.getDirection().dot(record.normal) / r.getDirection().length();
    }

    if (refract(r.getDirection(), outnormal, tmpior, refraction)) 
    {
        probability = fresnel(cosine, ior_);
    } else 
    {
        probability = 1;
    }

    if (randomFloat() < probability) 
    {
        scatter = Ray(record.p, reflection);
    } else 
    {
        scatter = Ray(record.p, refraction);
    }
    return true;

}