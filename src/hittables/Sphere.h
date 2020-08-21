#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "core/Vector3D.h"
#include "Hittable.h"

class Sphere : public Hittable {

public:
    /**
     * @param [in] center
     * @param [in] radius
     * @param [in] material
     */
    Sphere(const Vector3D &center, float radius, std::shared_ptr<Material> material);
    bool hit(const Ray &r, float cmin, float cmax, Hitrecord &record) const override;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const override;

private:
    const Vector3D center;
    float radius;
    std::shared_ptr<Material> matptr;

};


#endif //RAYTRACER_SPHERE_H
