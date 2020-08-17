#include "Metal.h"
#include "core/Utility.h"

Metal::Metal(const Vector3D &albedo, float blur) :
        albedo(albedo), blur(blur) {
    if (blur > 1) this->blur = 1;
}

bool Metal::scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                    std::function<float()> &randomFloat) const {

    Vector3D reflection = reflect(r.getDirection(), record.normal);

    if (blur > 0) {
        scatter = Ray(record.p, reflection + blur * randomInUnitSphere(randomFloat));
    } else {
        scatter = Ray(record.p, reflection);
    }

    attenuation = albedo;
    return ((scatter.getDirection().dot(record.normal)) > 0);
}