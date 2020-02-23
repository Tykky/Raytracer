/** @file */

#ifndef RAYTRACER_UTILITY_H
#define RAYTRACER_UTILITY_H

#include "vector3D.h"
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
vector3D skyGradient(const Ray &r);

vector3D randomInUnitSphere(std::function<float()> &randomFloat);

/** @brief Mirrors vector v with surface normal n */
vector3D reflect(const vector3D &v, const vector3D &n);

/** @brief Refracts vector according to snells law */
bool refract(const vector3D &v, const vector3D &n, float ior, vector3D &refraction);

#endif //RAYTRACER_UTILITY_H
