
.. _program_listing_file_Source_materials_Metal.cpp:

Program Listing for File Metal.cpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_materials_Metal.cpp>` (``Source\materials\Metal.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Metal.h"
   #include "core/Utility.h"
   
   Metal::Metal(const Vector3D &albedo, float blur) :
           albedo(albedo), blur(blur) {
       if (blur > 1) this->blur = 1;
   }
   
   bool Metal::scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                       std::function<float()> &randomFloat) const {
   
       Vector3D reflection = reflect(r.getDirection(), record.normal);
   
       if (blur > 0) {
           scatter = Ray(record.p, reflection + blur * randomInUnitSphere(randomFloat));
       } else {
           scatter = Ray(record.p, reflection);
       }
   
       attenuation = albedo;
       return ((scatter.getDirection().dot(record.normal)) > 0);
   }
