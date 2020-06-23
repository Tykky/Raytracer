#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Ray.h"
#include "Vector3D.h"

/** @brief Camera class translates screen coordinates to rays. */
class Camera {
private:

    // (width/height)
    float aspectratio;

    // fov in radians
    float theta;

    // half screen height
    float halfheight;

    // half screen width
    float halfwidth;

    Vector3D lowerleftcorner;

    // points at +Y on screen
    Vector3D vertical;

    // points at +X on screen
    Vector3D horizontal;

    Vector3D origin;

public:

    /**
     * @param fov = field of view (vertical) in degrees
     * @param aspectratio = screen width/height
     * @param pointat the direction camera is facing
     * @param up defines "up" direction for the camera. This
     * determines the roll angle.
     */
    Camera(float fov, float aspectratio, Vector3D origin, Vector3D pointat,
           Vector3D up);

    Ray getRay(float x, float y) const;
};


#endif //RAYTRACER_CAMERA_H
