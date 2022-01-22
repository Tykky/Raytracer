#include "Logging.h"
#include <iostream>

// TODO: consider using move semantics

void logMsg(const std::string& msg, MessageStore& messageStore)
{
    std::cout << "[INFO] " << msg << std::endl;
    messageStore.push_back({ msg, MessageType::INFO});
}

void logWarning(const std::string& msg, MessageStore& messageStore)
{
    std::cout << "[WARNING] " << msg << std::endl;
    messageStore.push_back({ msg, MessageType::WARNING});
}

void logError(const std::string& msg, MessageStore& messageStore)
{
    std::cout << "[ERROR] " << msg << std::endl;
    messageStore.push_back({ msg, MessageType::INFO});
}