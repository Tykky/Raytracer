#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include <utility>

#include "Primitive.h"
#include "hittables/Triangle.h"
#include "materials/Material.h"
#include "materials/Lambertian.h"
#include "hittables/Bvhnode.h"

class Mesh : public Primitive
{
public:
    Mesh(std::vector<std::shared_ptr<Primitive>>& triangles, std::function<float()>& randomFloat);
    virtual bool hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const;
    virtual bool boundingBox(float c0, float c1, Aabb& box) const;
    const std::vector<std::shared_ptr<Primitive>>& getTriangles() { return m_triangles; };

    /*
     * @brief Moves every vertex
     * @param offset from origin
     */
    void setOffset(const Vector3D& offset);

private:
    std::vector<std::shared_ptr<Primitive>> m_triangles = {};
    Bvhnode m_bvh;
    Vector3D m_offset = {};
    std::function<float()> m_randomFloat;
};


#endif // RAYTRACER_MESH_H