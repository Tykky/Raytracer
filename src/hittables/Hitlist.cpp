#include "Hitlist.h"

#include <utility>
#include "core/utility.h"

Hitlist::Hitlist(std::vector<std::shared_ptr<Hittable>> l) :
        m_list(std::move(l)) 
{}

bool Hitlist::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const 
{
    bool hit = false;
    Hitrecord tmprec;
    float closest = dmax;

    // test Ray r against all hittables
    for (int i = 0; i < m_list.size(); ++i) 
    {
        // Preserve only the closest hit
        if (m_list[i]->hit(r, dmin, closest, tmprec)) 
        {
            hit = true;
            closest = tmprec.distance;
            record = tmprec;
        }
    }
    return hit;
}

bool Hitlist::boundingBox(float c0, float c1, Aabb& box) const 
{
    if (m_list.empty()) { 
        return false;
    }

    Aabb tmpbox;
    bool first = m_list[0]->boundingBox(c0, c1, tmpbox);

    if (!first) return false;

    box = tmpbox;
    for (int i = 1; i < m_list.size(); ++i) 
    {
        if (m_list[i]->boundingBox(c0, c1, tmpbox)) 
        {
            box = surroundingBox(box,tmpbox);
        }
        else
        {
            return false;
        }
    }
    return true;
}
