#include <cmath>
#include "Sphere.h"

Sphere::Sphere(const Vector3D &center, float radius, Material* material) :
        m_center(center), m_radius(radius), m_matptr(material)
{}

bool Sphere::hit(const Ray& r, float dmin, float dmax, Hitrecord& record) const
{
	Vector3D oc = r.getPosition() - m_center;
    float a = r.getDirection().dot(r.getDirection());
    float b = oc.dot(r.getDirection());
    float c = oc.dot(oc) - m_radius * m_radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0) 
    {
        const float sqroot = std::sqrt(discriminant);
        float solution = (-b - sqroot) / a;
        if (solution < dmax && solution > dmin) 
        {
            record.distance = solution;
            record.p = r.pointAtDistance(record.distance);
            record.normal = (record.p - m_center) / m_radius;
            record.matptr = m_matptr;
            return true;
        }

        solution = (-b + sqroot) / a;
        if (solution < dmax && solution > dmin) 
        {
            record.distance = solution;
            record.p = r.pointAtDistance(record.distance);
            record.normal = (record.p - m_center) / m_radius;
            record.matptr = m_matptr;
            return true;
        }
    }
    return false;
}

bool Sphere::boundingBox(float c0, float c1, Aabb& box) const
{
    box = Aabb(m_center - Vector3D(m_radius, m_radius, m_radius),
               m_center + Vector3D(m_radius, m_radius, m_radius));
    return true;
}
