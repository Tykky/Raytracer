#ifndef RAYTRACER_HITLIST_H
#define RAYTRACER_HITLIST_H

#include <vector>
#include "Hittable.h"

/** @brief Hitlist is a data structure for keeping track of all hittables. */
class Hitlist : public Hittable 
{
public:
    /** @param list contains all hittables i.e geometry such as Spheres */
    Hitlist(std::vector<std::shared_ptr<Hittable>> list);
    virtual bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const;

private:
    std::vector<std::shared_ptr<Hittable>> list_;
};


#endif //RAYTRACER_HITLIST_H
