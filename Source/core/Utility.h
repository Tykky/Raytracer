#ifndef RAYTRACER_UTILITY_H
#define RAYTRACER_UTILITY_H

#include "Vector3D.h"
#include "Ray.h"
#include <functional>

/**
 * @brief Produces light blue -> white gradient.
 * @param r is Ray pointing at screen
 * @return returns color for each position on screen.
 */

inline Vector3D skyGradient(const Ray& r) {
    Vector3D direction = r.getDirection();
    direction.normalize();
    float t = 0.5 * (direction.getY() + 1);
    return (1.0 - t) * Vector3D(1.0, 1.0, 1.0) + t * Vector3D(0.5, 0.7, 1.0);
}

inline Vector3D randomInUnitSphere(std::function<float()>& randomFloat) {
    Vector3D point;
    while (true) {
        point = 2.0 * Vector3D(randomFloat(), randomFloat(), randomFloat()) - Vector3D(1, 1, 1);
        if (point.lengthSquared() < 1.0) {
            return point;
        }
    }
}

/**
 *  @brief Reflects/mirrors vector from surface
 *  when normal vector \f$ \vec{n} \f$ is known. 
 *  @param v is being mirrored
 *  @param n is normal vector
 */
inline Vector3D reflect(const Vector3D& v, const Vector3D& n) {
    return v - 2 * v.dot(n) * n;
}

/**
 *  @brief Refracts vector according to Snell's law
 *  @param v is being refracted
 *  @param n is normal vector
 *  @param ior is index of refraction
 *  @param refraction is is SET by this function.
 *  This contains the refracted vector that is produced by this function.
 *  Empty/zero vector should be given as reference parameter and the
 *  result should be used after the function has ran.
 *  @return true when vector refracts
 */
inline bool refract(const Vector3D& v, const Vector3D& n, float ior, Vector3D& refraction) {
    Vector3D uv = v;
    uv.normalize();
    float vdotn = uv.dot(n);
    float discriminant = 1.0 - ior * ior * (1 - vdotn * vdotn);
    if (discriminant > 0) {
        refraction = vdotn * (uv - n * vdotn) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

/**
 *  @brief Fresnel coefficient approximation by Christhophe Schlick
 *  @param cosine corresponds to cosine(theta) in formula
 *  @param ior = index of refraction (R0 in formula)
 *  @return fresnel coefficient
 */
inline float fresnel(float cosine, float ior) {
    float r0 = (1 - ior) / (1 + ior);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

#endif //RAYTRACER_UTILITY_H
