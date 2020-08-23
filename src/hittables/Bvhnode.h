#ifndef RAYTRACER_BVHNODE_H
#define RAYTRACER_BVHNODE_H

#include "hittables/Hittable.h"
#include <functional>

/** @brief Defines a node in bvh tree */
class Bvhnode : public Hittable {

public:
    Bvhnode();
    /**
     * @param [in] list
     * @param [in] start
     * @param [in] end
     * @param [in] c0
     * @param [in] c1
     * @param [in] randomFloat
     */
    Bvhnode(std::vector<std::shared_ptr<Hittable>> &list,
            size_t start, size_t end, float c0, float c1,
            std::function<float()> &randomFloat);
    virtual bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const;

private:
    std::shared_ptr<Hittable> right;
    std::shared_ptr<Hittable> left;
    Aabb node;
};


#endif //RAYTRACER_BVHNODE_H
