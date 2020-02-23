#include <primitives/Sphere.h>
#include <random>
#include <functional>

using namespace std;

vector3D skyGradient(const Ray &r) {
    vector3D direction = r.getDirection();
    direction.normalize();
    float t = 0.5 * (direction.getY() + 1);
    return (1.0 - t) * vector3D(1.0, 1.0, 1.0) + t * vector3D(0.5, 0.7, 1.0);
}

vector3D randomInUnitSphere(std::function<float()> &randomFloat) {
    vector3D point;
    while (true) {
        point = 2.0 * vector3D(randomFloat(), randomFloat(), randomFloat()) - vector3D(1, 1, 1);
        if (point.lengthSquared() < 1.0) {
            return point;
        }
    }
}

vector3D reflect(const vector3D &v, const vector3D &n) {
    return v - 2 * v.dot(n) * n;
}

bool refract(const vector3D &v, const vector3D &n, float ior, vector3D &refraction) {
    vector3D uv = v;
    uv.normalize();
    float vdotn = uv.dot(n);
    float discriminant = 1.0 - ior * ior * (1 - vdotn * vdotn);
    if (discriminant > 0) {
        refraction = vdotn * (uv - n * vdotn) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}
