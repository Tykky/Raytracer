#ifndef RAYTRACER_UTILITY_H
#define RAYTRACER_UTILITY_H

#include "Vector3D.h"
#include "Ray.h"
#include <functional>
#include <cmath>

/**
 * @brief Produces light blue -> white gradient
 * @param r is Ray pointing at screen
 * @return returns color for each position on screen
 */
Vector3D skyGradient(const Ray& r);

Vector3D randomInUnitSphere(std::function<float()>& randomFloat);

/**
 *  @param v is being mirrored
 *  @param n is normal vector
 */
Vector3D reflect(const Vector3D& v, const Vector3D& n);

/**
 *  @brief Refracts vector according to Snells law
 *  @param v is being refracted
 *  @param n is normal vector
 *  @param ior is index of refraction
 *  @param refraction contains the refracted vector
 *  @return true when vector refracts
 */
bool refract(const Vector3D& v, const Vector3D& n, float ior, Vector3D& refraction);

/**
 *  @brief Fresnel coefficient approximation by Christhophe Schlick
 *  @param cosine corresponds to cosine(theta) in formula
 *  @param ior = index of refraction (R0 in formula)
 *  @return fresnel coefficient
 */
float fresnel(float cosine, float ior);
#endif //RAYTRACER_UTILITY_H
