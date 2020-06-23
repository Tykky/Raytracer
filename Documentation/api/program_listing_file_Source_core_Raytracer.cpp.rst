
.. _program_listing_file_Source_core_Raytracer.cpp:

Program Listing for File Raytracer.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_core_Raytracer.cpp>` (``Source\core\Raytracer.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <random>
   #include <functional>
   #include <fstream>
   #include "Raytracer.h"
   #include "Utility.h"
   #include "materials/Material.h"
   
   Vector3D Raytracer::rayTrace(Ray& r, std::function<float()>& randomFloat) const {
       
       int depth = 0;
       hitrecord record;
       const float floaterror = 0.001;
       Vector3D color(1, 1, 1);
   
       while (world->hit(r, floaterror, std::numeric_limits<float>::max(), record)) {
           Ray scatter;
           Vector3D attenuation;
           if (depth < depthlimit && record.matptr->scatter(r, record, attenuation, scatter, randomFloat)) {
               depth++;
               r = scatter;
               color *= attenuation;
           } else {
               return {};
           }
       }
       if (depth == 0) {
           return skyGradient(r);
       }
       return color;
   }
   
   Raytracer::Raytracer(Primitive *world, const Camera &camera, int width, int height) :
           world(world), camera(camera), width(width), height(height), depthlimit(50) {
       framebuffer = std::make_unique<std::unique_ptr<Vector3D[]>[]>(height);
       for (int i = 0; i < height; ++i) {
           framebuffer[i] = std::make_unique<Vector3D[]>(width);
       }
   }
   
   void Raytracer::render(int samples) {
   #pragma omp parallel
       {
           // Each thread has it's own random generator
           std::mt19937 engine(1337);
           std::uniform_real_distribution<float> dist(0.0, 1.0);
           std::function<float()> randomFloat = bind(dist, engine);
   
   #pragma omp for
           // Loop through every pixel on screen
           for (int y = height - 1; y >= 0; --y) {
               for (int x = 0; x < width; ++x) {
                   Vector3D color = Vector3D();
                   // Sampling
                   for (int s = 0; s < samples; s++) {
                       Ray r = camera.getRay((float(x) + randomFloat()) / float(width),
                                             (float(y) + randomFloat()) / float(height));
                       color += rayTrace(r, randomFloat);
                   }
                   color /= float(samples);
                   // Write gamma corrected pixels to framebuffer
                   framebuffer[y][x] = Vector3D(sqrt(color.getR()), sqrt(color.getG()), sqrt(color.getB()));
               }
           }
       }
   }
   
   void Raytracer::frammebufferToNetpbm(std::string filename) {
       std::ofstream of;
       of.open(filename + ".ppm");
       of << "P3\n" << width << " " << height << "\n255\n";
       for (int y = height - 1; y >= 0; --y) {
           for (int x = 0; x < width; ++x) {
               of << int(framebuffer[y][x].getR() * 255.99) << " "
                  << int(framebuffer[y][x].getG() * 255.99) << " "
                  << int(framebuffer[y][x].getB() * 255.99) << "\n";
           }
       }
       of.close();
   }
   
   void Raytracer::bounceLimit(int limit) {
       depthlimit = limit;
   }
