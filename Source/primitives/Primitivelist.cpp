#include "Primitivelist.h"

Primitivelist::Primitivelist(Primitive **l, int n) :
        size(n), list(l) {
}

bool Primitivelist::hit(const Ray &r, float cmin, float cmax, hitrecord &record) const {

    bool hit = false;
    hitrecord tmprec;
    float closest = cmax;

    /* Test Ray r against all primitives in the world */
    for (int i = 0; i < size; ++i) {
        /* Preserve only closest hit  */
        if (list[i]->hit(r, cmin, closest, tmprec)) {
            hit = true;
            closest = tmprec.c;
            record = tmprec;
        }
    }
    return hit;
}
