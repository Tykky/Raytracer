#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Hittable.h"
#include "core/Vector3D.h"
#include "materials/Material.h"

class Triangle : public Hittable {
    public:
        Triangle() = default;
        Triangle(const Vector3D &v0, const Vector3D &v1, const Vector3D &v2, const Vector3D &n, Material *material) :
            vertex0_(v0), vertex1_(v1), vertex2_(v2), normal_(n), material_(material) {};

        virtual bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const override;
        virtual bool boundingBox(float c0, float c1, Aabb &box) const override;

        const Vector3D &getNormal() const { return normal_; }
        void setVertex(const Vector3D &vertex, int index);
        const Vector3D &getVertex(int index) const;
        void setMaterial(Material *material) { material_ = material; }
        void setNormal(Vector3D &n) { normal_ = n; }

        Triangle &operator+=(const Vector3D &rhs);

    private:
        Vector3D vertex0_ = {}, vertex1_ = {}, vertex2_ = {}, normal_ = {};
        Material *material_ = nullptr;

        Vector3D triangleMin() const;
        Vector3D triangleMax() const;
};


#endif //RAYTRACER_TRIANGLE_H