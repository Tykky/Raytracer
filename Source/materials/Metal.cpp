#include "Metal.h"
#include "Utility.h"

Metal::Metal(const vector3D &albedo, float blur) :
        albedo(albedo), blur(blur) {
    if (blur > 1) this->blur = 1;
}

bool Metal::scatter(const Ray &r, const hitrecord &record, vector3D &attenuation, Ray &scatter,
                    std::function<float()> &randomFloat) const {

    vector3D reflection = reflect(r.getDirection(), record.normal);

    if (blur > 0) {
        scatter = Ray(record.p, reflection + blur * randomInUnitSphere(randomFloat));
    } else {
        scatter = Ray(record.p, reflection);
    }

    attenuation = albedo;
    return ((scatter.getDirection().dot(record.normal)) > 0);
}