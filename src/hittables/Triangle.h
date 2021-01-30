#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Hittable.h"
#include "core/Vector3D.h"

class Triangle : public Hittable {
    public:
        Triangle();
        Triangle(Vector3D &v1, Vector3D &v2, Vector3D &v3);
        virtual bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const override;
        virtual bool boundingBox(float c0, float c1, Aabb &box) const override;
    private:
        Vector3D v1_, v2_, v3_;

};

#endif //RAYTRACER_TRIANGLE_H