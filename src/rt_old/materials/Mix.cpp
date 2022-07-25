#include "Mix.h"
#include "Metal.h"
#include "Lambertian.h"
#include "core/utility.h"

Mix::Mix(const Vector3D& lalbedo, const Vector3D& malbedo, float metalness, float roughness, float fresnelfactor,
           float ior) :
        m_lalbedo(lalbedo), m_malbedo(malbedo), m_metalness(metalness), m_fresnelfactor(fresnelfactor), m_ior(ior)
{
    m_lambertian = std::make_shared<Lambertian>(lalbedo);
    m_metal = std::make_shared<Metal>(malbedo, roughness);
}

bool Mix::scatter(const Ray& r, const Hitrecord& record, Vector3D& attenuation, Ray& scatter,
                  std::function<float()>& randomFloat) const
{
    float cosine = -r.getDirection().dot(record.normal) / r.getDirection().length();
    float probability = fresnel(cosine, m_ior);

    // Fresnel
    if (randomFloat() < m_fresnelfactor * probability)
    {
        attenuation = Vector3D(1,1,1); // no attenuation
        Vector3D reflection = reflect(r.getDirection(), record.normal);
        scatter = Ray(record.p, reflection);
        return true;
    }

    if(randomFloat() < m_metalness)
    {
        return m_metal->scatter(r, record, attenuation, scatter, randomFloat);
    }
    return m_lambertian->scatter(r, record, attenuation, scatter, randomFloat);
}

