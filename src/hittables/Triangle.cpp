# include "Triangle.h"

Triangle::Triangle() : v1_({}), v2_({}), v3_({}), material_(nullptr) {
}

Triangle::Triangle(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, Material *material) :
    v1_(v1), v2_(v2), v3_(v3), material_(material) {
}

bool Triangle::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const {
    // Möller-trumbore
    const float EPSILON = 0.0000001;
    Vector3D edge1 = v2_ - v1_;
    Vector3D edge2 = v3_ - v1_;
    Vector3D h = r.getDirection().cross(edge2);
    float a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON) {
        return false; // parallel to triangle
    }
    float f = 1.0f / a;
    Vector3D s = r.getPosition() - v1_;
    float u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f) {
        return false;
    }
    Vector3D q = s.cross(edge1);
    float v = f * r.getDirection().dot(q);
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }
    // Compute intersection point
    float t = f * edge2.dot(q);
    if (t > EPSILON) { // Intersect
        Vector3D outIntersectionPoint = r.getPosition() + r.getDirection() * t;
        record.distance = t;
        record.p = r.pointAtDistance(record.distance);
        record.normal = edge2.cross(edge1);
        record.matptr = material_;
        return true;
    } else {
        return false;
    }
}

bool Triangle::boundingBox(float c0, float c_1, Aabb &box) const {
    box = Aabb(Vector3D(-10,-10,-10), Vector3D(10,10,10));
    return true;
}