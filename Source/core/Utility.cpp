#include "Utility.h"

Vector3D skyGradient(const Ray& r) {
    Vector3D direction = r.getDirection();
    direction.normalize();
    float t = 0.5 * (direction.getY() + 1);
    return (1.0 - t) * Vector3D(1.0, 1.0, 1.0) + t * Vector3D(0.5, 0.7, 1.0);
}

Vector3D randomInUnitSphere(std::function<float()>& randomFloat) {
    Vector3D point;
    while (true) {
        point = 2.0 * Vector3D(randomFloat(), randomFloat(), randomFloat()) - Vector3D(1, 1, 1);
        if (point.lengthSquared() < 1.0) {
            return point;
        }
    }
}

Vector3D reflect(const Vector3D &v, const Vector3D &n) {
    return v - 2 * v.dot(n) * n;
}

bool refract(const Vector3D &v, const Vector3D &n, float ior, Vector3D &refraction) {
    Vector3D uv = v;
    uv.normalize();
    float vdotn = uv.dot(n);
    float discriminant = 1.0 - ior * ior * (1 - vdotn * vdotn);
    if (discriminant > 0) {
        refraction = vdotn * (uv - n * vdotn) - n * std::sqrt(discriminant);
        return true;
    }
    return false;
}

float fresnel(float cosine, float ior) {
    float r0 = (1 - ior) / (1 + ior);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}