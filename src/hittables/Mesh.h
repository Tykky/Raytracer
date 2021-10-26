#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include <utility>

#include "Hittable.h"
#include "hittables/Triangle.h"
#include "materials/Material.h"
#include "materials/Lambertian.h"
#include "hittables/Bvhnode.h"

class Mesh : public Hittable 
{
public:
    Mesh(std::vector<std::shared_ptr<Hittable>>& triangles, std::function<float()>& randomFloat);
    virtual bool hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const;
    virtual bool boundingBox(float c0, float c1, Aabb& box) const;
    const std::vector<std::shared_ptr<Hittable>>& getTriangles() { return m_triangles; };

    /*
     * @brief Moves every vertex
     * @param offset from origin
     */
    void setOffset(const Vector3D& offset);

private:
    std::vector<std::shared_ptr<Hittable>> m_triangles = {};
    Bvhnode m_bvh;
    Vector3D m_offset = {};
    std::function<float()> m_randomFloat;
};


#endif // RAYTRACER_MESH_H