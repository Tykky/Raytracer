#include "Dielectric.h"
#include "core/Utility.h"

Dielectric::Dielectric(float ior) :
        ior(ior) {
}

bool Dielectric::scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const {

    Vector3D outnormal;
    Vector3D reflection = reflect(r.getDirection(), record.normal);
    Vector3D refraction;

    float tmpior;

    // No attenuation
    attenuation = Vector3D(1.0, 1.0, 1.0);

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