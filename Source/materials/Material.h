#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "Ray.h"
#include "primitives/Primitive.h"
#include <functional>

/** @brief Material is abstract class which provides interface for all materials.
 *  @details Materials define how rays are scattered when they hit primitives. All
 *  materials have scatter() member function. */
class Material {
public:

    /** @param r ray that was tested against primitives.
     *  @param record contains the hitrecord where ray r hit.
     *  @param attenuation defines how much of the color/light is preserved after each bounce. (RGB vector)
     *  @param scatter is SET by this function. This contains the scatter ray that is produced by this function.
     *  Empty/zero vector should be given as reference parameter and the result should be used after
     *  the function has ran.
     *  @param randomFloat contains reference to random generator.
     *  @return True when scattered ray is valid  */
    virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                         std::function<float()> &randomFloat) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
