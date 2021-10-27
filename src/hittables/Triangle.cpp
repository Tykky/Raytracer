# include "Triangle.h"

const float DELTA = 0.01f;

bool Triangle::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const
{
    // Möller-trumbore
    const float EPSILON = 0.0000001;
    Vector3D edge1 = m_vertex1 - m_vertex0;
    Vector3D edge2 = m_vertex2 - m_vertex0;
    Vector3D h = r.getDirection().cross(edge2);
    float a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
    {
        return false; // parallel to triangle
    }
    float f = 1.0f / a;
    Vector3D s = r.getPosition() - m_vertex0;
    float u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f)
    {
        return false;
    }
    Vector3D q = s.cross(edge1);
    float v = f * r.getDirection().dot(q);
    if (v < 0.0f || u + v > 1.0f)
    {
        return false;
    }
    // Compute intersection point
    float t = f * edge2.dot(q);
    if (t > EPSILON)
    {
        record.distance = t;
        record.p = r.pointAtDistance(record.distance);
        record.normal = m_normal;
        record.matptr = m_material;
        return true;
    }
    else
    {
        return false;
    }
}

bool Triangle::boundingBox(float c0, float c_1, Aabb &box) const
{
    box = Aabb(triangleMin(), triangleMax());
    return true;
}

Vector3D Triangle::triangleMin() const
{
    auto minf3 = [](float a, float b , float c) {
        return a < b ? (a < c ? a : c) : (b < c ? b : c);
    };

    return Vector3D(minf3(m_vertex0.getX(), m_vertex1.getX(), m_vertex2.getX()) - DELTA,
                    minf3(m_vertex0.getY(), m_vertex1.getY(), m_vertex2.getY()) - DELTA,
                    minf3(m_vertex0.getZ(), m_vertex1.getZ(), m_vertex2.getZ()) - DELTA);
}

Vector3D Triangle::triangleMax() const
{
    auto maxf3 = [](float a, float b, float c)
    {
        return a > b ? (a > c ? a : c) : (b > c ? b : c);
    };

    return Vector3D(maxf3(m_vertex0.getX(), m_vertex1.getX(), m_vertex2.getX()) + DELTA,
                    maxf3(m_vertex0.getY(), m_vertex1.getY(), m_vertex2.getY()) + DELTA,
                    maxf3(m_vertex0.getZ(), m_vertex1.getZ(), m_vertex2.getZ()) + DELTA);
}

void Triangle::setVertex(const Vector3D& vertex, int index)
{
    switch(index)
    {
        case 0: m_vertex0 = vertex; break;
        case 1: m_vertex1 = vertex; break;
        case 2: m_vertex2 = vertex; break;
        default:
            std::cout << "Invalid index when setting vertex" << index << std::endl;
            exit(1);
    }
}

const Vector3D &Triangle::getVertex(int index) const
{
    switch(index)
    {
        case 0: return m_vertex0;
        case 1: return m_vertex1;
        case 2: return m_vertex2;
        default:
            std::cout << "Invalid index when getting vertex: " << index << std::endl;
            exit(1);
    }
}

Triangle& Triangle::operator+=(const Vector3D& rhs)
{
    m_vertex0 += rhs;
    m_vertex1 += rhs;
    m_vertex2 += rhs;
    return *this;
}