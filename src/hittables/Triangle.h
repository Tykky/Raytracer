#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Hittable.h"
#include "core/Vector3D.h"
#include "materials/Material.h"

class Triangle : public Hittable {
    public:
        Triangle() = default;
        Triangle(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2, const Vector3D& n, Material* material) :
                m_vertex0(v0), m_vertex1(v1), m_vertex2(v2), m_normal(n), m_material(material) {};

        virtual bool hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const override;
        virtual bool boundingBox(float c0, float c1, Aabb &box) const override;

        const Vector3D& getNormal() const { return m_normal; }
        void setVertex(const Vector3D& vertex, int index);
        const Vector3D& getVertex(int index) const;
        void setMaterial(Material* material) { m_material = material; }
        void setNormal(Vector3D& n) { m_normal = n; }

        Triangle &operator+=(const Vector3D &rhs);

    private:
        Vector3D m_vertex0 = {}, m_vertex1 = {}, m_vertex2 = {}, m_normal = {};
        Material* m_material = nullptr;

        Vector3D triangleMin() const;
        Vector3D triangleMax() const;
};


#endif //RAYTRACER_TRIANGLE_H