#ifndef RAYTRACER_IO_H
#define RAYTRACER_IO_H

#include <vector>

typedef std::vector<char> Rawbuffer;

// returns null terminated file
bool readRawFile(Rawbuffer& buffer, const char* filename);

#endif //RAYTRACER_IO_H
