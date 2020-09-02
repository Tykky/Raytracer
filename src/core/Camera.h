#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Ray.h"
#include "Vector3D.h"

/** @brief Camera class translates screen coordinates to rays. */
class Camera {

public:
    Camera();
    /**
     * @param [in] fov = field of view (vertical) in degrees
     * @param [in] aspectratio = screen width/height
     * @param [in] pointat the direction camera is facing
     * @param [in] up defines "up" direction for the camera. This
     * determines the roll angle.
     */
    Camera(float fov, float aspectratio,const Vector3D &origin,const Vector3D &pointat,
           const Vector3D &up);

    /**
     * @param [in] x
     * @param [in] y
     */
    Ray getRay(const float &x, const float &y) const;
    void setFov(const float &fov);
    void setPos(const Vector3D &pos);
    void setUp(const Vector3D &up);
    void setPointAt(const Vector3D &lookat);
    void setAspectRatio(const float &ratio);

    /**
     * @brief Updates all vectors used internally. Call this
     * when you want to apply changes to fov, position, up vector
     * and aspectratio.
     */
    void applyChanges();

private:
    float aspectratio_;
    float theta_;
    float halfheight_;
    float halfwidth_;
    Vector3D lowerleftcorner_;
    Vector3D vertical_;
    Vector3D horizontal_;
    Vector3D origin_;
    Vector3D pointat_;
    Vector3D up_;

};


#endif //RAYTRACER_CAMERA_H
