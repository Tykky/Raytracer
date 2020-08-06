#ifndef RAYTRACER_BVHNODE_H
#define RAYTRACER_BVHNODE_H

#include "primitives/Primitive.h"
#include <functional>


bool boxCompare(const std::shared_ptr<Primitive> &a, const std::shared_ptr<Primitive> &b,
                int axis);

/** @brief Defines a node in bvh tree */
class Bvhnode : public Primitive {

private:
    std::shared_ptr<Primitive> right;
    std::shared_ptr<Primitive> left;
    Aabb node;
	
public:
    Bvhnode();
    Bvhnode(std::vector<std::shared_ptr<Primitive>> &list,
            size_t start, size_t end, float c0, float c1,
            std::function<float()> &randomFloat);
    virtual bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const;

};


#endif //RAYTRACER_BVHNODE_H
