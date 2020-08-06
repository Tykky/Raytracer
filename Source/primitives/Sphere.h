#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "core/Vector3D.h"
#include "Primitive.h"

class Sphere : public Primitive {
private:
    const Vector3D center;
    float radius;
    std::shared_ptr<Material> matptr;
public:
    Sphere(const Vector3D &center, float radius, std::shared_ptr<Material> mat);
	/**
	 * @brief Solves the ray intersection equation
	 * @param cmin is minimum acceptable solution for ray intersection equation
     * @param cmax is maximum acceptable solution for ray intersection equation
     * @param record is SET by this member function. Empty hitrecord
     * should be given as reference and used after function has ran.
     * @return true when hit equation has solution between cmin and cmax, otherwise
     * false.
	 */
    bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const override;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const override;
};


#endif //RAYTRACER_SPHERE_H
