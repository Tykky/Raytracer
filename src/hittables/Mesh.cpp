#include "Mesh.h"

Mesh::Mesh(std::vector<std::shared_ptr<Hittable>> &triangles, std::function<float()> &randomFloat) : triangles_(triangles), randomFloat_(randomFloat) 
{
    bvh_ = Bvhnode(triangles_, 0, triangles_.size(), 0, 1, randomFloat_);
};

bool Mesh::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const 
{
    return bvh_.hit(r, dmin, dmax, record);
}

bool Mesh::boundingBox(float c0, float c1, Aabb &box) const 
{
    return bvh_.boundingBox(c0, c1, box);
}

void Mesh::setOffset(const Vector3D &offset) 
{
    const auto delta = offset - offset_;
    for(const auto &hittable: triangles_) 
    {
        auto *triangle = dynamic_cast<Triangle *>(hittable.get());
        (*triangle) += delta;
    }
    offset_ = offset;
    bvh_ = Bvhnode(triangles_, 0, triangles_.size(), 0, 1, randomFloat_);
}