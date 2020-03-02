/** @file */

#ifndef RAYTRACER_UTILITY_H
#define RAYTRACER_UTILITY_H

#include "Vector3D.h"
#include "Ray.h"
#include "primitives/Primitive.h"
#include "materials/Material.h"
#include <random>
#include <functional>

/**@brief Produces light blue -> white gradient.
 * @details is used to produce "sky" like background
 * when rays do not hit any objects in the world.
 * @param r is Ray pointing at screen
 * @return returns color for each position on screen.  */
Vector3D skyGradient(const Ray &r);

Vector3D randomInUnitSphere(std::function<float()> &randomFloat);

/** @brief Mirrors vector v with surface normal n */
Vector3D reflect(const Vector3D &v, const Vector3D &n);

/** @brief Refracts vector according to snells law */
bool refract(const Vector3D &v, const Vector3D &n, float ior, Vector3D &refraction);

/** @brief Fresnel coefficient approximation by Christhophe Schlick
 *  @details https://en.wikipedia.org/wiki/Schlick%27s_approximation
 *  @param cosine corresponds to cosine(theta) in formula
 *  @param ior = index of refraction (R0 in formula)
 *  @return fresnel coefficient */
float fresnel(float cosine, float ior);

#endif //RAYTRACER_UTILITY_H
