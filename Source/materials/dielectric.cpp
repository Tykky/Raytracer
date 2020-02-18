#include "dielectric.h"
#include "utility.h"

dielectric::dielectric(float ior) :
        ior(ior) {
}

bool dielectric::scatter(const ray &r, const hitrecord &record, vector3D &attenuation, ray &scatter,
                         std::function<float()> &randomFloat) const {

    vector3D outnormal;
    vector3D reflection = reflect(r.getB(), record.normal);
    vector3D refraction;

    float tmpior;

    // No attenuation
    attenuation = vector3D(1.0,1.0,1.0);

    float probability; // reflect probability
    float cosine;

    if(r.getB().dot(record.normal) > 0) { // Enters object
        outnormal = -record.normal;
        tmpior = ior;
        cosine = ior*r.getB().dot(record.normal) / r.getB().length();
    } else { // Exits object
        outnormal = record.normal;
        tmpior = 1.0 / ior;
        cosine = -r.getB().dot(record.normal) / r.getB().length();
    }

    if(refract(r.getB(), outnormal, tmpior, refraction)) { // Refract
        probability = fresnel(cosine, ior);
    } else { // Reflect
        probability = 1;
    }

    if(randomFloat() < probability) { // reflect
        scatter = ray(record.p, reflection);
    } else {
        scatter = ray(record.p, refraction);
    }
    return true;

}

float fresnel(float cosine, float ior) {
    float r0 = (1 - ior) / (1 + ior);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
