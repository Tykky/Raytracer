/** Primitivelist is a datastructure for keeping track of all primitives/objects
 * in the world. */

#ifndef RAYTRACER_PRIMITIVELIST_H
#define RAYTRACER_PRIMITIVELIST_H

#include "primitive.h"

class primitivelist : public primitive {
private:
    int size;
    primitive **list;

public:
    primitivelist(primitive **l, int n);
    /** Computes hits for all primitives in list */
    virtual bool hit(const ray &r, float cmin, float cmax, hitrecord &record) const;
};


#endif //RAYTRACER_PRIMITIVELIST_H
