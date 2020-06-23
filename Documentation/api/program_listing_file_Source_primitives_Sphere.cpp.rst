
.. _program_listing_file_Source_primitives_Sphere.cpp:

Program Listing for File Sphere.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_primitives_Sphere.cpp>` (``Source\primitives\Sphere.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <cmath>
   #include "Sphere.h"
   
   Sphere::Sphere(const Vector3D &center, float radius, Material *mat) :
           center(center), radius(radius), matptr(mat) {
   }
   
   bool Sphere::hit(const Ray &r, float cmin, float cmax, hitrecord &record) const {
   
       Vector3D oc = r.getOrigin() - center;
       float a = r.getDirection().dot(r.getDirection());
       float b = oc.dot(r.getDirection());
       float c = oc.dot(oc) - radius * radius;
       float discriminant = b * b - a * c;
   
       if (discriminant > 0) {
           const float sqroot = sqrt(discriminant);
           float solution = (-b - sqroot) / a;
           if (solution < cmax && solution > cmin) {
               record.c = solution;
               record.p = r.pointAtC(record.c);
               record.normal = (record.p - center) / radius;
               record.matptr = matptr;
               return true;
           }
   
           solution = (-b + sqroot) / a;
           if (solution < cmax && solution > cmin) {
               record.c = solution;
               record.p = r.pointAtC(record.c);
               record.normal = (record.p - center) / radius;
               record.matptr = matptr;
               return true;
           }
       }
       return false;
   }
   
   bool Sphere::boundingBox(float c0, float c1, Aabb &box) const {
       box = Aabb(center - Vector3D(radius,radius,radius),
                  center + Vector3D(radius,radius,radius));
       return true;
   }
