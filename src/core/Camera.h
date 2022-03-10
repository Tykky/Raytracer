#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Ray.h"
#include "Vector3D.h"
#include <functional>

class RTCamera
{
public:
    /**
     * @param fov = field of view (vertical) in degrees
     * @param aspectratio = screen width/height
     * @param pointat the direction camera is facing
     * @param up defines "up" direction for the camera. This
     * determines the roll angle.
     */
    RTCamera();
    RTCamera(float fov, float aspectratio, const Vector3D& origin,const Vector3D& pointat,
           const Vector3D& up);

    Ray getRay(float x, float y) const;
    Ray getRayScreenCoords(int x, int y, unsigned int width, unsigned int height) const;
    Ray getRayScreenCoordsWithJitter(int x, int y, unsigned int width, unsigned int height, std::function<float()>& randomFloat) const;
    void setFov(const float& fov);
    void setPos(const Vector3D& pos);
    void setUp(const Vector3D& up);
    void setAspectRatio(const float& ratio);

    /**
     * @brief Updates all vectors used internally. Call this
     * when you want to apply changes to fov, position, up vector
     * and aspectratio.
     */
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
