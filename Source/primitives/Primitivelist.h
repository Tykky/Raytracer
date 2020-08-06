#ifndef RAYTRACER_PRIMITIVELIST_H
#define RAYTRACER_PRIMITIVELIST_H

#include <vector>
#include "Primitive.h"

/** @brief Primitivelist is a data structure for keeping track of all primitives. */



class Primitivelist : public Primitive {
private:
    std::vector<std::shared_ptr<Primitive>> list;

public:
    Primitivelist(std::vector<std::shared_ptr<Primitive>> list);
    virtual bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const;
};


#endif //RAYTRACER_PRIMITIVELIST_H
