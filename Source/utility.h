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
vector3D recursiveScatter(const ray &r, primitive *world, int depth);
inline float randomFloat() {
    static std::mt19937 engine;
    static std::uniform_real_distribution<float> dist(0.0,1.0);
    static std::function<float()> roll = std::bind(dist,engine);
    return roll();
}
vector3D randomInUnitSphere();

#endif //RAYTRACER_UTILITY_H
