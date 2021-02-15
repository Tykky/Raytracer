# include "Triangle.h"

namespace {
    const float DELTA = 0.01f;
}

bool Triangle::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const {
    // Möller-trumbore
    const float EPSILON = 0.0000001;
    Vector3D edge1 = vertex1_ - vertex0_;
    Vector3D edge2 = vertex2_ - vertex0_;
    Vector3D h = r.getDirection().cross(edge2);
    float a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON) {
        return false; // parallel to triangle
    }
    float f = 1.0f / a;
    Vector3D s = r.getPosition() - vertex0_;
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
    box = Aabb(triangleMin(), triangleMax());
    return true;
}

Vector3D Triangle::triangleMin() const {
    auto minf3 = [](float a, float b , float c) {
        return a < b ? (a < c ? a : c) : (b < c ? b : c);
    };

    return Vector3D(minf3(vertex0_.getX(), vertex1_.getX(), vertex2_.getX()) - DELTA,
                    minf3(vertex0_.getY(), vertex1_.getY(), vertex2_.getY()) - DELTA,
                    minf3(vertex0_.getZ(), vertex1_.getZ(), vertex2_.getZ()) - DELTA);
}

Vector3D Triangle::triangleMax() const {
    auto maxf3 = [](float a, float b, float c) {
        return a > b ? (a > c ? a : c) : (b > c ? b : c);
    };

    return Vector3D(maxf3(vertex0_.getX(), vertex1_.getX(), vertex2_.getX()) + DELTA,
                    maxf3(vertex0_.getY(), vertex1_.getY(), vertex2_.getY()) + DELTA,
                    maxf3(vertex0_.getZ(), vertex1_.getZ(), vertex2_.getZ()) + DELTA);
}

void Triangle::setVertex(const Vector3D &vertex, int index) {
    switch(index) {
        case 0: vertex0_ = vertex; break;
        case 1: vertex1_ = vertex; break;
        case 2: vertex2_ = vertex; break;
        default:
            std::cout << "Invalid index when setting vertex" << index << std::endl;
            exit(1);
    }
}

const Vector3D &Triangle::getVertex(int index) const {
    switch(index) {
        case 0: return vertex0_;
        case 1: return vertex1_;
        case 2: return vertex2_;
        default:
            std::cout << "Invalid index when getting vertex: " << index << std::endl;
            exit(1);
    }
}

Triangle &Triangle::operator+=(const Vector3D &rhs) {
    vertex0_ += rhs;
    vertex1_ += rhs;
    vertex2_ += rhs;
    return *this;
}