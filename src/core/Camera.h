#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Ray.h"
#include "Vector3D.h"

class Camera 
{
public:
    Camera();
    Camera(float fov, float aspectratio, const Vector3D& origin,const Vector3D& pointat,
           const Vector3D& up);

    Ray getRay(const float& x, const float& y) const;
    void setFov(const float& fov);
    void setPos(const Vector3D& pos);
    void setUp(const Vector3D& up);
    void setAspectRatio(const float& ratio);

    // Updates all changes made to fov, pos, up or aspect ratio
    void applyChanges();

private:
    float m_aspectRatio;
    float m_theta;
    float m_halfHeight;
    float m_halfWidth;
    Vector3D m_lowerLeftCorner;
    Vector3D m_vertical;
    Vector3D m_horizontal;
    Vector3D m_origin;
    Vector3D m_pointat;
    Vector3D m_up;
};

#endif //RAYTRACER_CAMERA_H
