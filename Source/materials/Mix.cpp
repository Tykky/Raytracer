#include "Mix.h"
#include "Metal.h"
#include "Lambertian.h"
#include "core/Utility.h"

Mix::Mix(const Vector3D &lalbedo, const Vector3D &malbedo, float metalness, float roughness, float fresnelfactor,
           float ior) :
        lalbedo(lalbedo), malbedo(malbedo), metalness(metalness), fresnelfactor(fresnelfactor), ior(ior)  {

    pLambertian = std::make_unique<Lambertian>(this->lalbedo);
    pMetal = std::make_unique<Metal>(this->malbedo,roughness);

}

bool Mix::scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                   std::function<float()> &randomFloat) const {

    float cosine = -r.getDirection().dot(record.normal) / r.getDirection().length();
    float probability = fresnel(cosine,ior);

    // Fresnel
    if(randomFloat() < fresnelfactor*probability) {
        attenuation = Vector3D(1,1,1); // no attenuation
        Vector3D reflection = reflect(r.getDirection(), record.normal);
        scatter = Ray(record.p, reflection);
        return true;
    }

    if(randomFloat() < metalness) {
        return pMetal->scatter(r,record,attenuation,scatter,randomFloat);
    }
	
    return pLambertian->scatter(r,record,attenuation,scatter,randomFloat);
}
