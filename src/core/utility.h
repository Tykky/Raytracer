#ifndef RAYTRACER_UTILITY_H
#define RAYTRACER_UTILITY_H

#include "Vector3D.h"
#include "Ray.h"
#include <functional>
#include <cmath>

/**
 * @brief Produces light blue -> white gradient
 * @param [in] r is Ray pointing at screen
 * @return returns color for each position on screen
 */

Vector3D skyGradient(const Ray& r);

Vector3D randomInUnitSphere(std::function<float()>& randomFloat);

/**
 *  @brief Reflects/mirrors vector from surface
 *  when normal vector is known
 *  @param [in] v is being mirrored
 *  @param [in] n is normal vector
 */
Vector3D reflect(const Vector3D& v, const Vector3D& n);

/**
 *  @brief Refracts vector according to Snell's law
 *  @param [in] v is being refracted
 *  @param [in] n is normal vector
 *  @param [in] ior is index of refraction
 *  @param [out] refraction contains the refracted vector
 *  @return true when vector refracts
 */
bool refract(const Vector3D& v, const Vector3D& n, float ior, Vector3D& refraction);

/**
 *  @brief Fresnel coefficient approximation by Christhophe Schlick
 *  @details https://en.wikipedia.org/wiki/Schlick%27s_approximation
 *  @param [in] cosine corresponds to cosine(theta) in formula
 *  @param [in] ior = index of refraction (R0 in formula)
 *  @return fresnel coefficient
 */
float fresnel(float cosine, float ior);

#endif //RAYTRACER_UTILITY_H
