#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H

#include "Material.h"

/**
 * @brief Dielectric material allows rays to pass through objects.
 * Useful for water, glass etc..
 */
class Dielectric : public Material
{
    public:
        /** @param ior = index of refraction */
        Dielectric(float ior);

        virtual bool scatter(const Ray& r, const Hitrecord& record, Vector3D& attenuation, Ray& scatter,
                            std::function<float()>& randomFloat) const;

    private:
        const float m_ior;

};


#endif //RAYTRACER_DIELECTRIC_H
