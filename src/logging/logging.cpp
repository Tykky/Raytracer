#include "logging.h"
#include <iostream>

void logMsg(const char *msg)
{
    std::cout << "[INFO] " << msg << std::endl;
}

void logWarning(const char *msg)
{

    std::cout << "[WARNING] " << msg << std::endl;
}

void logError(const char *msg)
{
    std::cout << "[ERROR] " << msg << std::endl;
}
