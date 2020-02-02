/** @brief camera class translates screen coordinates to rays. */
/** Screen coordinates are represented as x in [0,1], y in [0,1],
 *  when x = 1 (right edge of the screen) y = 1 (top edge of the screen),
 *  x=0, y=0 is bottom left of the screen. */

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "ray.h"
#include "vector3D.h"


class camera {
private:

    /** field of view (horizontal in degrees) */
    float fov;

    /** aspect ratio (width/height) */
    float aspectratio;

    /** vertical angle in radians */
    float theta;

    /** half screen height */
    float halfheight;

    /** half screen width */
    float halfwidth;

    /** lower left corner of screen */
    vector3D lowerleftcorner;

    /** points at +Y on screen */
    vector3D vertical;

    /** points at +X on screen */
    vector3D horizontal;

public:

    /** Initializes fov, aspect ratio and computes the vectors
     * that define the virtual screen. */
    camera(float fov, float aspectratio);

    /** Returns ray for each (x,y) screen coordinate */
    ray getRay(float x, float y) const;
};


#endif //RAYTRACER_CAMERA_H
