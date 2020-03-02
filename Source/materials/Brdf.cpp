#include "Brdf.h"
#include "Utility.h"

Brdf::Brdf(const Vector3D &lalbedo, const Vector3D &malbedo, float metalness, float roughness, float fresnelfactor,
           float ior) :
        lalbedo(lalbedo), malbedo(malbedo), metalness(metalness), fresnelfactor(fresnelfactor), ior(ior)  {

    lptr = make_unique<Lambertian>(this->lalbedo);
    mptr = make_unique<Metal>(this->malbedo,roughness);

}

bool Brdf::scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                   std::function<float()> &randomFloat) const {

    Vector3D direction = r.getDirection();
    direction.normalize();

    float cosine = direction.dot(record.normal);
    float probability = fresnel(cosine,ior);

    

    if(randomFloat() < metalness) {
        return mptr->scatter(r,record,attenuation,scatter,randomFloat);
    } else {
        return lptr->scatter(r,record,attenuation,scatter,randomFloat);
    }

}

