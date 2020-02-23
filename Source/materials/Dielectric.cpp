#include "Dielectric.h"
#include "Utility.h"

Dielectric::Dielectric(float ior) :
        ior(ior) {
}

bool Dielectric::scatter(const Ray &r, const hitrecord &record, vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const {

    vector3D outnormal;
    vector3D reflection = reflect(r.getDirection(), record.normal);
    vector3D refraction;

    float tmpior;

    // No attenuation
    attenuation = vector3D(1.0, 1.0, 1.0);

    float probability; // reflect probability
    float cosine;

    if (r.getDirection().dot(record.normal) > 0) { // Exits object
        outnormal = -record.normal;
        tmpior = ior;
        cosine = ior * r.getDirection().dot(record.normal) / r.getDirection().length();
    } else { // Enters object
        outnormal = record.normal;
        tmpior = 1.0 / ior;
        cosine = -r.getDirection().dot(record.normal) / r.getDirection().length();
    }

    if (refract(r.getDirection(), outnormal, tmpior, refraction)) { // Refract
        probability = fresnel(cosine, ior);
    } else { // Reflect
        probability = 1;
    }

    if (randomFloat() < probability) { // reflect
        scatter = Ray(record.p, reflection);
    } else {
        scatter = Ray(record.p, refraction);
    }
    return true;

}

float fresnel(float cosine, float ior) {
    float r0 = (1 - ior) / (1 + ior);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
