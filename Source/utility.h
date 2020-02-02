/** @brief utility contains useful static "helper" functions. */

#ifndef RAYTRACER_UTILITY_H
#define RAYTRACER_UTILITY_H

#include "vector3D.h"
#include "ray.h"

/**@brief Produces light blue -> white gradient. */
/** is used to produce "sky" like background
 * when rays do not hit any objects in the world. */
vector3D skyGradient(const ray &r);


#endif //RAYTRACER_UTILITY_H
