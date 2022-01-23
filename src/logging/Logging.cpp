#include "Logging.h"
#include <iostream>

static MessageStore MESSAGE_STORE;

void logMsg(std::string&& msg)
{
    std::cout << "[INFO] " << msg << std::endl;
    MESSAGE_STORE.push_back({ std::move(msg), MessageType::INFO});
}

void logMsg(std::string& msg)
{
    std::cout << "[INFO] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::INFO});
}

void logWarning(std::string&& msg)
{
    std::cout << "[WARNING] " << msg << std::endl;
    MESSAGE_STORE.push_back({ std::move(msg), MessageType::WARNING});
}

void logWarning(std::string& msg)
{
    std::cout << "[WARNING] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::WARNING});
}

void logError(std::string&& msg)
{
    std::cout << "[ERROR] " << msg << std::endl;
    MESSAGE_STORE.push_back({ std::move(msg), MessageType::INFO});
}

void logError(std::string& msg)
{
    std::cout << "[ERROR] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::INFO});
}

const Message* getLogMessages(int& size)
{
    size = MESSAGE_STORE.size();
    return MESSAGE_STORE.data();
}

void clearLogs()
{
    MESSAGE_STORE.clear();
}
