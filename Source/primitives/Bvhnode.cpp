#include "Bvhnode.h"
#include "core/Utility.h"

// All of the compare methods are the
// same except they do comparison in different axis.

int boxCompareX(const void *a, const void *b) {
    Aabb left, right;
    Primitive *ah = *(Primitive**)a;
    Primitive *bh = *(Primitive**)b;

    if(!ah->boundingBox(0,0, left) || !bh->boundingBox(0,0,right)) {
        std::cerr << "Creation of bounding box failed in compare X\n";
    }

    if(left.getMin().getX() - right.getMin().getX() < 0) {
        return -1;
    }
    return 1;

}

int boxCompareY(const void *a, const void *b) {
    Aabb left, right;
    Primitive *ah = *(Primitive**)a;
    Primitive *bh = *(Primitive**)b;

    if(!ah->boundingBox(0,0, left) || !bh->boundingBox(0,0,right)) {
        std::cerr << "Creation of bounding box failed in compare Y\n";
    }

    if(left.getMin().getY() - right.getMin().getY() < 0) {
        return -1;
    }
    return 1;
}

int boxCompareZ(const void *a, const void *b) {
    Aabb left, right;
    Primitive *ah = *(Primitive**)a;
    Primitive *bh = *(Primitive**)b;

    if(!ah->boundingBox(0,0, left) || !bh->boundingBox(0,0,right)) {
        std::cerr << "Creation of bounding box failed in compare Z\n";
    }

    if(left.getMin().getZ() - right.getMin().getZ() < 0) {
        return -1;
    }
    return 1;
}


Bvhnode::Bvhnode() {}


Bvhnode::Bvhnode(Primitive **list, int n, float c0, float c1, std::function<float()> &randomFloat) {

    // Division is made by
    // 1. Randomly choose an axis (from 3)
    // 2. Sort the primitives
    // 3. half each sub tree

    // TODO: replace sorting algorithm with self implemented one

    int axis = int(3*randomFloat());

    if(axis == 0) {
        qsort(list,n, sizeof(Primitive *),boxCompareX);
    } else if(axis == 1) {
        qsort(list, n, sizeof(Primitive *), boxCompareY);
    } else {
        qsort(list, n ,sizeof(Primitive *), boxCompareZ);
    }

    if(n == 1) {
        pLeft = list[0];
        pRight = list[0];
    } else if(n == 2) {
        pLeft = list[0];
        pRight = list[1];
    } else {
        pLeft = new Bvhnode(list, n/2, c0, c1, randomFloat);
        pRight = new Bvhnode(list + n/2, n - n/2, c0, c1 ,randomFloat);
    }

    Aabb leftbox;
    Aabb rightbox;

    if(!pLeft->boundingBox(c0,c1,leftbox) ||
       !pRight->boundingBox(c0,c1,rightbox)) {
        std::cerr << "Creation of bounding box failed in Bvhnode \n";
    }

    node = surroundingBox(leftbox,rightbox);

}

bool Bvhnode::hit(const Ray &r, float cmin, float cmax, hitrecord &record) const {

    // Do search in bvh
    if(node.hit(r,cmin,cmax)) {
        hitrecord leftrec, rightrec;
        bool hitleft = pLeft->hit(r,cmin,cmax, leftrec);
        bool hitright = pRight->hit(r,cmin,cmax,rightrec);
        if(hitleft && hitright) {
            if(leftrec.c < rightrec.c) {
                record = leftrec;
            } else {
                record = rightrec;
            }
            return true;
        }
    	if(hitleft) {
            record = leftrec;
            return true;
        }
    	if(hitright) {
            record = rightrec;
            return true;
        }
    }
    return false;
}

bool Bvhnode::boundingBox(float c0, float c1, Aabb &box) const {
    box = node;
    return true;
}