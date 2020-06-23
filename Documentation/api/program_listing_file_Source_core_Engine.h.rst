
.. _program_listing_file_Source_core_Engine.h:

Program Listing for File Engine.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_core_Engine.h>` (``Source\core\Engine.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_ENGINE_H
   #define RAYTRACER_ENGINE_H
   
   #include <memory>
   #include "primitives/Primitive.h"
   #include "Vector3D.h"
   #include "Camera.h"
   
   class Engine {
   
   private:
       Primitive *world;
       std::unique_ptr<std::unique_ptr<Vector3D[]>[]> framebuffer;
       Camera camera;
       int width;
       int height;
       int depthlimit;
   
       Vector3D rayTrace(Ray& r, std::function<float()>& randomFloat) const;
       
   public:
       Engine(Primitive *world, const Camera &camera, int width, int height);
   
       void render(int samples);
   
       /* @brief stores frambuffer data to .ppm file */
       void frammebufferToNetpbm(std::string filename);
   
       void bounceLimit(int limit);
   
   };
   
   
   #endif //RAYTRACER_ENGINE_H
