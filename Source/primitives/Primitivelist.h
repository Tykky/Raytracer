#ifndef RAYTRACER_PRIMITIVELIST_H
#define RAYTRACER_PRIMITIVELIST_H

#include "Primitive.h"

/** @brief Primitivelist is a data structure for keeping track of all primitives
 * in the world. */
class Primitivelist : public Primitive {
private:
    int size;
    Primitive **list;

public:
    Primitivelist(Primitive **l, int n);

    /** @brief  Computes hits for all primitives in list and
     *  picks the one closest to the Camera.
     *  @param record contains the closest hit.
     *  @return true if any of the primitives is hit in the list,
     *  otherwise false. */
    virtual bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const;
};


#endif //RAYTRACER_PRIMITIVELIST_H
