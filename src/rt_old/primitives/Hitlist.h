#ifndef RAYTRACER_HITLIST_H
#define RAYTRACER_HITLIST_H

#include <vector>
#include "Primitive.h"

/** @brief Hitlist is a data structure for keeping track of all hittables. */
class Hitlist : public Primitive
{
public:
    Hitlist(std::vector<std::shared_ptr<Primitive>> list);
    virtual bool hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const;
    virtual bool boundingBox(float c0, float c1, Aabb& box) const;

private:
    std::vector<std::shared_ptr<Primitive>> m_list;
};

#endif //RAYTRACER_HITLIST_H
