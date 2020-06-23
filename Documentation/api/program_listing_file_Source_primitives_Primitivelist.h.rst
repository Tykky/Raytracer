
.. _program_listing_file_Source_primitives_Primitivelist.h:

Program Listing for File Primitivelist.h
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_primitives_Primitivelist.h>` (``Source\primitives\Primitivelist.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_PRIMITIVELIST_H
   #define RAYTRACER_PRIMITIVELIST_H
   
   #include "Primitive.h"
   
   class Primitivelist : public Primitive {
   private:
       int size;
       Primitive **list;
   
   public:
       Primitivelist(Primitive **l, int n);
       virtual bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const;
       virtual bool boundingBox(float c0, float c1, Aabb &box) const;
   };
   
   
   #endif //RAYTRACER_PRIMITIVELIST_H
