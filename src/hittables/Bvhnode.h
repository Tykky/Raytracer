#ifndef RAYTRACER_BVHNODE_H
#define RAYTRACER_BVHNODE_H

#include "hittables/Primitive.h"
#include <functional>

class Bvhnode : public Primitive
{
public:
    Bvhnode();
    Bvhnode(std::vector<std::shared_ptr<Primitive>>& list,
            size_t start, size_t end, float c0, float c1,
            std::function<float()>& randomFloat);

    virtual bool hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const;
    virtual bool boundingBox(float c0, float c1, Aabb& box) const;

private:
    std::shared_ptr<Primitive> m_right;
    std::shared_ptr<Primitive> m_left;
    Aabb m_node;
};


#endif //RAYTRACER_BVHNODE_Hear
