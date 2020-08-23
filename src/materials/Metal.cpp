#include "Metal.h"
#include "core/Utility.h"

Metal::Metal(const Vector3D &albedo, float blur) :
        albedo_(albedo), blur_(blur) {
    if (blur > 1) this->blur_ = 1;
}

bool Metal::scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                    std::function<float()> &randomFloat) const {

    Vector3D reflection = reflect(r.getDirection(), record.normal);

    if (blur_ > 0) {
        scatter = Ray(record.p, reflection + blur_ * randomInUnitSphere(randomFloat));
    } else {
        scatter = Ray(record.p, reflection);
    }

    attenuation = albedo_;
    return ((scatter.getDirection().dot(record.normal)) > 0);
}