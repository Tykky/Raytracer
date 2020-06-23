#ifndef RAYTRACER_PRIMITIVELIST_H
#define RAYTRACER_PRIMITIVELIST_H

#include "Primitive.h"

/** @brief Primitivelist is a data structure for keeping track of all primitives. */
class Primitivelist : public Primitive {
private:
    int size;
    Primitive **list;

public:
    Primitivelist(Primitive **l, int n);
    virtual bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const;
};


#endif //RAYTRACER_PRIMITIVELIST_H
