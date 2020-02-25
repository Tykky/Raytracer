/** @file */

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Ray.h"
#include "vector3D.h"

/** @brief Camera class translates screen coordinates to rays.
 *  @details Screen coordinates are represented as x in [0,1], y in [0,1],
 *  when x = 1 (right edge of the screen) y = 1 (top edge of the screen),
 *  x=0, y=0 is bottom left of the screen. */
class Camera {
private:

    // field of view (horizontal in degrees)
    float fov;

    // aspect ratio (width/height)
    float aspectratio;

    // fov in radians
    float theta;

    // half screen height
    float halfheight;

    // half screen width
    float halfwidth;

    // coordinates of lower left corner of screen
    vector3D lowerleftcorner;

    // points at +Y on screen
    vector3D vertical;

    // points at +X on screen
    vector3D horizontal;

    vector3D origin;

public:

    /** @brief Initializes fov, aspect ratio and computes the vectors
     * that define the virtual screen.
     * @param fov = field of view (vertical) in degrees
     * @param aspectratio = screen width/height
     * @param pointat the direction camera is facing
     * @param up defines "up" direction for the camera. This
     * determines the roll angle. */
    Camera(float fov, float aspectratio, vector3D origin, vector3D pointat,
            vector3D up);

    /** @return corresponding Ray for each (x,y) screen coordinate */
    Ray getRay(float x, float y) const;
};


#endif //RAYTRACER_CAMERA_H
