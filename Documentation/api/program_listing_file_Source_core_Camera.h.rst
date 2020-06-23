
.. _program_listing_file_Source_core_Camera.h:

Program Listing for File Camera.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_core_Camera.h>` (``Source\core\Camera.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_CAMERA_H
   #define RAYTRACER_CAMERA_H
   
   #include "Ray.h"
   #include "Vector3D.h"
   
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
   
       Camera(float fov, float aspectratio, Vector3D origin, Vector3D pointat,
              Vector3D up);
   
       Ray getRay(float x, float y) const;
   };
   
   
   #endif //RAYTRACER_CAMERA_H
