#ifndef RAYTRACER_OBJLOADER_H
#define RAYTRACER_OBJLOADER_H

#include "hittables/Mesh.h"
#include <string>

Mesh loadObj(const std::string &file);


#endif // RAYTRACER_OBJLOADER_H