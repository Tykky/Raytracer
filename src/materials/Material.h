#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "core/Ray.h"
#include "hittables/Hittable.h"
#include <functional>
#include <memory>

/** @brief Abstract class which provides interface for all materials. */
class Material 
{
public:
    /**
     * @param r is being scattered
     * @param record contains intersection details
     * @param attenuation coefficient tells how much of the light
     * is absorbed during scatter
     * @param scatter is the ray that is scattered outwards from
     * the intersection point.
     * @param randomFloat contains reference to random generator
     * @return true if ray is scattered successfully
     */
    virtual bool scatter(const Ray &r, const Hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
