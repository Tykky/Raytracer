#include "Logging.h"
#include <iostream>
#include <vector>

// We store all log messages here
static std::vector<Message> MESSAGE_STORE;

void logMsg(const char* msg)
{
    std::cout << "[INFO] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::INFO});
}

void logWarning(const char* msg)
{
    std::cout << "[WARNING] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::WARNING});
}

void logError(const char* msg)
{
    std::cout << "[ERROR] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::INFO});
}
