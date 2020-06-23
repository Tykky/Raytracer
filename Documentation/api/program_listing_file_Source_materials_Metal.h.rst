
.. _program_listing_file_Source_materials_Metal.h:

Program Listing for File Metal.h
================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_materials_Metal.h>` (``Source\materials\Metal.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_METAL_H
   #define RAYTRACER_METAL_H
   
   #include "Material.h"
   
   class Metal : public Material {
   
   private:
       const Vector3D albedo;
       float blur;
   
   public:
       Metal(const Vector3D &albedo, float b);
   
       virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                            std::function<float()> &randomFloat) const;
   };
   
   
   #endif //RAYTRACER_METAL_H
