
.. _program_listing_file_Source_primitives_Aabb.h:

Program Listing for File Aabb.h
===============================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_primitives_Aabb.h>` (``Source\primitives\Aabb.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_AABB_H
   #define RAYTRACER_AABB_H
   
   #include "core/Vector3D.h"
   #include "core/Ray.h"
   
   // Simple min & max functions for floats
   // These should be faster than standard fmax & fmin.
   
   inline float ffmin(float a, float b) {
       return a < b ? a : b;
   }
   
   inline float ffmax(float a, float b) {
       return a > b ? a : b;
   }
   
   class Aabb {
   
   private:
       Vector3D min;
       Vector3D max;
   
   public:
       Aabb();
       
       Aabb(const Vector3D &a, const Vector3D &b);
       Vector3D getMin();
       Vector3D getMax();
   
       inline bool hit(const Ray &r, float cmin, float cmax) const {
           // Source: Ray Tracing In The Next Week
           for (int a = 0; a < 3; a++) {
               float invD = 1.0f / r.getDirection()[a];
               float t0 = (min[a] - r.getOrigin()[a]) * invD;
               float t1 = (max[a] - r.getOrigin()[a]) * invD;
               if (invD < 0.0f)
                   std::swap(t0, t1);
               cmin = t0 > cmin ? t0 : cmin;
               cmax = t1 < cmax ? t1 : cmax;
               if (cmax <= cmin)
                   return false;
           }
           return true;
       }
   };
   
   Aabb surroundingBox(Aabb box1, Aabb box2);
   
   
   #endif //RAYTRACER_AABB_H
