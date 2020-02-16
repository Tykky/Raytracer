/** @brief utility contains useful static "helper" functions. */

#ifndef RAYTRACER_UTILITY_H
#define RAYTRACER_UTILITY_H

#include "vector3D.h"
#include "ray.h"
#include "primitives/primitive.h"
#include "materials/material.h"
#include <random>
#include <functional>

/**@brief Produces light blue -> white gradient. */
/** is used to produce "sky" like background
 * when rays do not hit any objects in the world. */
vector3D skyGradient(const ray &r);
vector3D sphereNormalColor(hitrecord record);
vector3D recursiveScatter(const ray &r, primitive *world, int depth, std::function<float()> &randomFloat);
vector3D randomInUnitSphere(std::function<float()> &randomFloat);

#endif //RAYTRACER_UTILITY_H
