
.. _program_listing_file_Source_core_Ray.h:

Program Listing for File Ray.h
==============================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_core_Ray.h>` (``Source\core\Ray.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_RAY_H
   #define RAYTRACER_RAY_H
   
   #include "Vector3D.h"
   
   class Ray {
   
   private:
       Vector3D origin;
       Vector3D direction;
       
   public:
       Ray();
       Ray(const Vector3D &origin, const Vector3D &direction);
       Vector3D getOrigin() const;
       Vector3D getDirection() const;
   
       Vector3D pointAtC(float c) const;
   };
   
   #endif //RAYTRACER_RAY_H
