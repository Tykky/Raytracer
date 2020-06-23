
.. _program_listing_file_Source_primitives_Sphere.h:

Program Listing for File Sphere.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_primitives_Sphere.h>` (``Source\primitives\Sphere.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_SPHERE_H
   #define RAYTRACER_SPHERE_H
   
   #include "core/Vector3D.h"
   #include "Primitive.h"
   class Sphere : public Primitive {
   private:
       const Vector3D center;
       float radius;
       Material *matptr;
   public:
       Sphere(const Vector3D &center, float radius, Material *mat);
       bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const override;
       virtual bool boundingBox(float c0, float c1, Aabb &box) const override;
   };
   
   
   #endif //RAYTRACER_SPHERE_H
