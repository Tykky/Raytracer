#ifndef RAYTRACER_IO_H
#define RAYTRACER_IO_H

#include <vector>
#include <string>

typedef std::vector<char> Rawbuffer;
typedef std::string FilePath;
typedef std::string FileExtension;

// returns null terminated file
bool readRawFile(Rawbuffer& buffer, const char* filename);

// Returns all file paths inside given directory, ignores folders
std::vector<FilePath> getFilePaths(std::string& path);

FileExtension getFileExtension(FilePath path);

#endif //RAYTRACER_IO_H
