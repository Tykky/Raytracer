#include "Io.h"
#include <fstream>
#include <filesystem>
#include "logging/Logging.h"

bool readRawFile(Rawbuffer& buffer, const char* filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (file.fail())
    {
        RT_LOG_ERROR("Failed to read file {}", filename);
        return false;
    }
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize size = file.gcount();
    buffer.resize(size);
    file.clear();
    file.seekg(0, std::ios_base::beg);
    file.read(buffer.data(), size);
    buffer.push_back('\0');
    return true;
}

std::vector<FilePath> getFilePaths(std::string& path)
{
    std::size_t numEntries = 0;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        numEntries++;
    }

    std::vector<FilePath> filePaths(numEntries);
    int i = 0;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        filePaths[i] = entry.path().string();
        i++;
    }

    return filePaths;
}

FileExtension getFileExtension(FilePath path)
{
    return std::filesystem::path(path).extension().string();
}
