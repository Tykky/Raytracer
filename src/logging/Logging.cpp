#include "Logging.h"
#include <iostream>

void logMsg(std::string&& msg, MessageStore& messageStore)
{
    logMsg(msg);
    messageStore.push_back({ std::move(msg), MessageType::INFO});
}

void logMsg(std::string& msg)
{
    std::cout << "[INFO] " << msg << std::endl;
}

void logWarning(std::string&& msg, MessageStore& messageStore)
{
    logWarning(msg);
    messageStore.push_back({ std::move(msg), MessageType::WARNING});
}

void logWarning(std::string& msg)
{
    std::cout << "[WARNING] " << msg << std::endl;
}
void logError(std::string&& msg, MessageStore& messageStore)
{
    logError(msg);
    messageStore.push_back({ std::move(msg), MessageType::INFO});
}

void logError(std::string& msg)
{
    std::cout << "[ERROR] " << msg << std::endl;
}