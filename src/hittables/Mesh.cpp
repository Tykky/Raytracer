#include "Mesh.h"

Mesh::Mesh(std::vector<std::shared_ptr<Hittable>> &triangles, std::function<float()> &randomFloat) : m_triangles(triangles), m_randomFloat(randomFloat)
{
    m_bvh = Bvhnode(m_triangles, 0, m_triangles.size(), 0, 1, randomFloat);
}

bool Mesh::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const 
{
    return m_bvh.hit(r, dmin, dmax, record);
}

bool Mesh::boundingBox(float c0, float c1, Aabb &box) const 
{
    return m_bvh.boundingBox(c0, c1, box);
}

void Mesh::setOffset(const Vector3D &offset) 
{
    const auto delta = offset - m_offset;
    for(const auto &hittable : m_triangles)
    {
        auto *triangle = dynamic_cast<Triangle *>(hittable.get());
        (*triangle) += delta;
    }
    m_offset = offset;
    m_bvh = Bvhnode(m_triangles, 0, m_triangles.size(), 0, 1, m_randomFloat);
}