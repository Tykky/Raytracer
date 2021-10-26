#include "Metal.h"
#include "core/utility.h"

Metal::Metal(const Vector3D& albedo, float blur) :
        m_albedo(albedo), m_blur(blur)
{
    if (blur > 1) this->m_blur = 1;
}

bool Metal::scatter(const Ray& r, const Hitrecord& record, Vector3D& attenuation, Ray& scatter,
                    std::function<float()>& randomFloat) const
{
    Vector3D reflection = reflect(r.getDirection(), record.normal);

    if (m_blur > 0)
    {
        scatter = Ray(record.p, reflection + m_blur * randomInUnitSphere(randomFloat));
    }
    else
    {
        scatter = Ray(record.p, reflection);
    }

    attenuation = m_albedo;
    return ((scatter.getDirection().dot(record.normal)) > 0);
}