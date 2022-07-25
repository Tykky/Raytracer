#include "Dielectric.h"
#include "core/utility.h"

Dielectric::Dielectric(float ior) :
        m_ior(ior)
{}

bool Dielectric::scatter(const Ray& r, const Hitrecord& record, Vector3D& attenuation, Ray& scatter,
                         std::function<float()>& randomFloat) const
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
        tmpior = m_ior;
        cosine = m_ior * r.getDirection().dot(record.normal) / r.getDirection().length();
    }
    else
    {
        outnormal = record.normal;
        tmpior = 1.0 / m_ior;
        cosine = -r.getDirection().dot(record.normal) / r.getDirection().length();
    }

    if (refract(r.getDirection(), outnormal, tmpior, refraction))
    {
        probability = fresnel(cosine, m_ior);
    }
    else
    {
        probability = 1;
    }

    if (randomFloat() < probability)
    {
        scatter = Ray(record.p, reflection);
    }
    else
    {
        scatter = Ray(record.p, refraction);
    }
    return true;

}