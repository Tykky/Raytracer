# include "Triangle.h"

namespace {
    const float DELTA = 0.01f;
}

Triangle::Triangle() : vertex1_({}), vertex2_({}), vertex3_({}), material_(nullptr) {
}

Triangle::Triangle(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, const Vector3D &n, Material *material) :
    vertex1_(v1), vertex2_(v2), vertex3_(v3), material_(material), normal_(n) {
}

bool Triangle::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const {
    // Möller-trumbore
    const float EPSILON = 0.0000001;
    Vector3D edge1 = vertex2_ - vertex1_;
    Vector3D edge2 = vertex3_ - vertex1_;
    Vector3D h = r.getDirection().cross(edge2);
    float a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON) {
        return false; // parallel to triangle
    }
    float f = 1.0f / a;
    Vector3D s = r.getPosition() - vertex1_;
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
        record.normal = normal_;
        record.matptr = material_;
        return true;
    } else {
        return false;
    }
}

bool Triangle::boundingBox(float c0, float c_1, Aabb &box) const {
    auto vecmin = triangleMin();
    auto vecmax = triangleMax();
    
    box = Aabb(triangleMin(), triangleMax());
    return true;
}

Vector3D Triangle::triangleMin() const {
    auto minf3 = [](float a, float b , float c) {
        return a < b ? (a < c ? a : c) : (b < c ? b : c);
    };

    return Vector3D(minf3(vertex1_.getX(), vertex2_.getX(), vertex3_.getX()) - DELTA,
                    minf3(vertex1_.getY(), vertex2_.getY(), vertex3_.getY()) - DELTA,
                    minf3(vertex1_.getZ(), vertex2_.getZ(), vertex3_.getZ()) - DELTA);
}

Vector3D Triangle::triangleMax() const {
    auto maxf3 = [](float a, float b, float c) {
        return a > b ? (a > c ? a : c) : (b > c ? b : c);
    };

    return Vector3D(maxf3(vertex1_.getX(), vertex2_.getX(), vertex3_.getX()) + DELTA,
                    maxf3(vertex1_.getY(), vertex2_.getY(), vertex3_.getY()) + DELTA,
                    maxf3(vertex1_.getZ(), vertex2_.getZ(), vertex3_.getZ()) + DELTA);
}