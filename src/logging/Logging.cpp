#include "Logging.h"
#include <iostream>
#include <cassert>

// We store all log messages here
static MessageStore MESSAGE_STORE;

void logMsg(const char* msg)
{
    assert(msg);
    std::cout << "[INFO] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::INFO});
}

void logWarning(const char* msg)
{
    assert(msg);
    std::cout << "[WARNING] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::WARNING});
}

void logError(const char* msg)
{
    assert(msg);
    std::cout << "[ERROR] " << msg << std::endl;
    MESSAGE_STORE.push_back({ msg, MessageType::INFO});
}

MessageStore* getLogMessages()
{
    return &MESSAGE_STORE;
}