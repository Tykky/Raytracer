#include "Mix.h"
#include "Metal.h"
#include "Lambertian.h"
#include "core/utility.h"

Mix::Mix(const Vector3D &lalbedo, const Vector3D &malbedo, float metalness, float roughness, float fresnelfactor,
           float ior) :
    lalbedo_(lalbedo), malbedo_(malbedo), metalness_(metalness), fresnelfactor_(fresnelfactor), ior_(ior)  
{
    lambertian_ = std::make_shared<Lambertian>(lalbedo);
    metal_ = std::make_shared<Metal>(malbedo, roughness);
}

bool Mix::scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                  std::function<float()> &randomFloat) const {

    float cosine = -r.getDirection().dot(record.normal) / r.getDirection().length();
    float probability = fresnel(cosine, ior_);

    // Fresnel
    if(randomFloat() < fresnelfactor_ * probability) {
        attenuation = Vector3D(1,1,1); // no attenuation
        Vector3D reflection = reflect(r.getDirection(), record.normal);
        scatter = Ray(record.p, reflection);
        return true;
    }

    if(randomFloat() < metalness_) 
    {
        return metal_->scatter(r, record, attenuation, scatter, randomFloat);
    }
    return lambertian_->scatter(r, record, attenuation, scatter, randomFloat);
}

