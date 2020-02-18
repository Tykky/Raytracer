#include "metal.h"
#include "utility.h"

metal::metal(const vector3D &albedo, float blur) :
    albedo(albedo), blur(blur) {
    if(blur > 1) this->blur = 1;
}

bool metal::scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                    std::function<float()> &randomFloat) const {

    vector3D reflection = reflect(r.getB(),record.normal);

    if(blur > 0) {
        scatter = ray(record.p, reflection + blur * randomInUnitSphere(randomFloat));
    } else {
        scatter = ray(record.p, reflection);
    }

    attenuation = albedo;
    return ((scatter.getB().dot(record.normal)) > 0);
}