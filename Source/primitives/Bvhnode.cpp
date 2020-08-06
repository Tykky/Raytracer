#include <algorithm>
#include "Bvhnode.h"
#include "core/Utility.h"
#include "primitives/Primitivelist.h"

bool boxCompare(const std::shared_ptr<Primitive> &a, const std::shared_ptr<Primitive> &b, int axis) {

    Aabb box_a;
    Aabb box_b;

    if(!a->boundingBox(0,0,box_a) || !b->boundingBox(0,0, box_b)) {
        std::cerr << "[ERROR] No bounding box in bvh constructor" << std::endl;
    }

    return box_a.getMin()[axis] < box_b.getMin()[axis];
}

Bvhnode::Bvhnode() {}

Bvhnode::Bvhnode(std::vector<std::shared_ptr<Primitive>> &list,
                 size_t start, size_t end, float c0, float c1,
                 std::function<float()> &randomFloat) {

    size_t span = end - start;

    // Comparators for each axis
    const std::function<bool(std::shared_ptr<Primitive>, std::shared_ptr<Primitive>)> comparators[] = {
        {[](auto a, auto b) { return boxCompare(a, b, 0);}}, // x
        {[](auto a, auto b) { return boxCompare(a, b, 1);}}, // y
        {[](auto a, auto b) { return boxCompare(a, b, 2);}}  // z
    };

    // Division is made by
    // 1. Randomly choose an axis (from 3)
    // 2. Sort the primitives
    // 3. half each sub tree

    int axis = static_cast<int>(3 * randomFloat());
    auto comparator = comparators[axis];

    if(span == 1) {
        left = list[start];
        right = list[start];
    } else if(span == 2) {
        if(comparator(list[start], list[start + 1])) {
            left = list[start];
            right = list[start + 1];
        } else {
            right = list[start];
            left = list[start + 1];
        }
    } else {
        std::sort(list.begin() + start, list.begin() + end, comparator);
        auto middle = start + span/2;
        left = std::make_shared<Bvhnode>(list, start, middle, c0, c1, randomFloat);
        right = std::make_shared<Bvhnode>(list, middle, end, c0, c1, randomFloat);
    }

    Aabb leftbox;
    Aabb rightbox;

    if(!left->boundingBox(c0, c1, leftbox) ||
       !right->boundingBox(c0, c1, rightbox)) {
        std::cerr << "[ERROR] Creation of bounding box failed in Bvhnode" << std::endl;
    }

    node = surroundingBox(leftbox,rightbox);

}

bool Bvhnode::hit(const Ray &r, float cmin, float cmax, hitrecord &record) const {

    // Do search in bvh
    if(node.hit(r,cmin,cmax)) {
        hitrecord leftrec, rightrec;
        bool hitleft = left->hit(r, cmin, cmax, leftrec);
        bool hitright = right->hit(r, cmin, cmax, rightrec);
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
