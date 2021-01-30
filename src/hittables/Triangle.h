#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Hittable.h"
#include "core/Vector3D.h"
#include "materials/Material.h"

class Triangle : public Hittable {
    public:
        Triangle();
        Triangle(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, Material *material);
        virtual bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const override;
        virtual bool boundingBox(float c0, float c1, Aabb &box) const override;
    private:
        Vector3D vertex1_, vertex2_, vertex3_;
        Material *material_;

        Vector3D triangleMin() const;
        Vector3D triangleMax() const;

};

#endif //RAYTRACER_TRIANGLE_H