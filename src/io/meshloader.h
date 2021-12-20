#ifndef RAYTRACER_OBJLOADER_H
#define RAYTRACER_OBJLOADER_H

#include "primitives//Mesh.h"
#include <string>

std::vector<std::shared_ptr<Primitive>> loadObj(const std::string& file, Material* material, std::function<float()> randomFloat);


#endif // RAYTRACER_OBJLOADER_H