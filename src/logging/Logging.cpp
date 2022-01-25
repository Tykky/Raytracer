#include "Logging.h"
#include <iostream>

static MessageStore MESSAGE_STORE;

void logMsg(std::string&& msg)
{
    auto tmp = "[INFO] " + std::move(msg);
    std::cout << tmp << std::endl;
    MESSAGE_STORE.push_back({ std::move(tmp), MessageType::INFO});
}

void logMsg(std::string& msg)
{
    auto tmp = "[INFO] " + msg;
    std::cout << tmp << std::endl;
    MESSAGE_STORE.push_back({ std::move(tmp), MessageType::INFO});
}

void logWarning(std::string&& msg)
{
    auto tmp = "[WARNING] " + std::move(msg);
    std::cout << tmp << std::endl;
    MESSAGE_STORE.push_back({ std::move(tmp), MessageType::WARNING});
}

void logWarning(std::string& msg)
{
    auto tmp = "[WARNING] " + msg;
    std::cout << tmp << std::endl;
    MESSAGE_STORE.push_back({ std::move(tmp), MessageType::WARNING});
}

void logError(std::string&& msg)
{
    auto tmp = "[WRANGING] " + msg;
    std::cout << tmp << std::endl;
    MESSAGE_STORE.push_back({ std::move(tmp), MessageType::ERROR});
}
void logError(std::string& msg)
{
    auto tmp = "[ERROR] " + msg;
    std::cout << tmp << std::endl;
    MESSAGE_STORE.push_back({ std::move(tmp), MessageType::ERROR});
}

MessageStore& getLogMessages()
{
    return MESSAGE_STORE;
}