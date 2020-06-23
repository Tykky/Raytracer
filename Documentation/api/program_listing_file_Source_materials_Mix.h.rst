
.. _program_listing_file_Source_materials_Mix.h:

Program Listing for File Mix.h
==============================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_materials_Mix.h>` (``Source\materials\Mix.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_BRDF_H
   #define RAYTRACER_BRDF_H
   
   #include <memory>
   #include "Material.h"
   
   class Mix : public Material {
   
   private:
       std::unique_ptr<Material> pMetal;
       std::unique_ptr<Material> pLambertian; 
       const Vector3D malbedo;
       const Vector3D lalbedo;
       float metalness;
       float fresnelfactor;
       float ior;
   
   public:
       Mix(const Vector3D &lalbedo, const Vector3D &malbedo, float metalness, float roughness, float fresnel,
            float ior);
   
       virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                            std::function<float()> &randomFloat) const;
   
   };
   
   
   #endif //RAYTRACER_BRDF_H
