#include <algorithm>
#include "Bvhnode.h"
#include "core/utility.h"

namespace {

    bool boxCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, int axis) 
    {
        Aabb box_a;
        Aabb box_b;
        
        if (!a->boundingBox(0, 0, box_a) || !b->boundingBox(0, 0, box_b)) {
            std::cerr << "[ERROR] No bounding box in bvh constructor" << std::endl;
        }
        return box_a.getMin()[axis] < box_b.getMin()[axis];
    }

}

Bvhnode::Bvhnode() {}

Bvhnode::Bvhnode(std::vector<std::shared_ptr<Hittable>>& list,
                 size_t start, size_t end, float c0, float c1,
                 std::function<float()>& randomFloat) {

    size_t span = end - start;

    const std::function<bool(std::shared_ptr<Hittable>, std::shared_ptr<Hittable>)> comparators[] = 
    {
        {[](auto a, auto b) { return boxCompare(a, b, 0);}}, // x
        {[](auto a, auto b) { return boxCompare(a, b, 1);}}, // y
        {[](auto a, auto b) { return boxCompare(a, b, 2);}}  // z
    };

    // Division is made by
    // 1. Randomly choose an axis (from 3)
    // 2. Sort the hittables
    // 3. half each sub tree

    int axis = static_cast<int>(3 * randomFloat());
    auto comparator = comparators[axis];

    if (span == 1) 
    {
        m_left = list[start];
        m_right = list[start];
    } 
    else if (span == 2) 
    {
        if (comparator(list[start], list[start + 1])) 
        {
            m_left = list[start];
            m_right = list[start + 1];
        }
        else
        {
            m_right = list[start];
            m_left = list[start + 1];
        }
    } 
    else 
    {
        std::sort(list.begin() + start, list.begin() + end, comparator);
        auto middle = start + span / 2;
        m_left = std::make_shared<Bvhnode>(list, start, middle, c0, c1, randomFloat);
        m_right = std::make_shared<Bvhnode>(list, middle, end, c0, c1, randomFloat);
    }

    Aabb leftbox;
    Aabb rightbox;

    if (!m_left->boundingBox(c0, c1, leftbox) || !m_right->boundingBox(c0, c1, rightbox)) 
    {
        std::cerr << "[ERROR] Creation of bounding box failed in Bvhnode" << std::endl;
    }
    m_node = surroundingBox(leftbox, rightbox);
}

bool Bvhnode::hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const 
{
    if (m_node.hit(r, dmin, dmax)) 
    {
        Hitrecord leftrec, rightrec;
        bool hitleft = m_left->hit(r, dmin, dmax, leftrec);
        bool hitright = m_right->hit(r, dmin, dmax, rightrec);
        if (hitleft && hitright) {
            if (leftrec.distance < rightrec.distance) 
            {
                record = leftrec;
            }
            else
            {
                record = rightrec;
            }
            return true;
        }
    	if (hitleft) 
        {
            record = leftrec;
            return true;
        }
    	if (hitright) 
        {
            record = rightrec;
            return true;
        }
    }
    return false;
}

bool Bvhnode::boundingBox(float c0, float c1, Aabb &box) const 
{
    box = m_node;
    return true;
}