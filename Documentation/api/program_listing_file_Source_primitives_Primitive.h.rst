
.. _program_listing_file_Source_primitives_Primitive.h:

Program Listing for File Primitive.h
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_primitives_Primitive.h>` (``Source\primitives\Primitive.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_PRIMITIVE_H
   #define RAYTRACER_PRIMITIVE_H
   
   #include "core/Ray.h"
   #include "Aabb.h"
   
   class Material;
   
   struct hitrecord {
   
       float c;
   
       Vector3D p;
   
       Vector3D normal;
   
       Material *matptr;
   };
   
   class Primitive {
   public:
   
       virtual bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const = 0;
   
       virtual bool boundingBox(float c0, float c1, Aabb &box) const = 0;
   
   };
   
   
   #endif //RAYTRACER_PRIMITIVE_H
