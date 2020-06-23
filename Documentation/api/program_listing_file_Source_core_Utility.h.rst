
.. _program_listing_file_Source_core_Utility.h:

Program Listing for File Utility.h
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_core_Utility.h>` (``Source\core\Utility.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_UTILITY_H
   #define RAYTRACER_UTILITY_H
   
   #include "Vector3D.h"
   #include "Ray.h"
   #include <functional>
   
   inline Vector3D skyGradient(const Ray& r) {
       Vector3D direction = r.getDirection();
       direction.normalize();
       float t = 0.5 * (direction.getY() + 1);
       return (1.0 - t) * Vector3D(1.0, 1.0, 1.0) + t * Vector3D(0.5, 0.7, 1.0);
   }
   
   inline Vector3D randomInUnitSphere(std::function<float()>& randomFloat) {
       Vector3D point;
       while (true) {
           point = 2.0 * Vector3D(randomFloat(), randomFloat(), randomFloat()) - Vector3D(1, 1, 1);
           if (point.lengthSquared() < 1.0) {
               return point;
           }
       }
   }
   
   inline Vector3D reflect(const Vector3D& v, const Vector3D& n) {
       return v - 2 * v.dot(n) * n;
   }
   
   inline bool refract(const Vector3D& v, const Vector3D& n, float ior, Vector3D& refraction) {
       Vector3D uv = v;
       uv.normalize();
       float vdotn = uv.dot(n);
       float discriminant = 1.0 - ior * ior * (1 - vdotn * vdotn);
       if (discriminant > 0) {
           refraction = vdotn * (uv - n * vdotn) - n * sqrt(discriminant);
           return true;
       }
       return false;
   }
   
   inline float fresnel(float cosine, float ior) {
       float r0 = (1 - ior) / (1 + ior);
       r0 = r0 * r0;
       return r0 + (1 - r0) * pow((1 - cosine), 5);
   }
   
   #endif //RAYTRACER_UTILITY_H
