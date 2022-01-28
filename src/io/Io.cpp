#include "Io.h"
#include <fstream>

bool readRawFile(Rawbuffer& buffer, const char* filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (file.fail())
        return false;
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize size = file.gcount();
    buffer.resize(size);
    file.clear(); // ignore sets eof
    file.seekg(0, std::ios_base::beg);
    file.read(buffer.data(), size);
}