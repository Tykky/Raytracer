#ifndef RAYTRACER_OBJLOADER_H
#define RAYTRACER_OBJLOADER_H

#include "hittables/Mesh.h"
#include <string>

std::vector<std::shared_ptr<Hittable>> loadObj(const std::string& file, Material* material, std::function<float()> randomFloat);


#endif // RAYTRACER_OBJLOADER_H