#ifndef RAYTRACER_LOGGING_H
#define RAYTRACER_LOGGING_H

#include <vector>
#include <string>

#ifdef WIN32
#undef ERROR
#endif

// Cursed ms macro. We need to undefine this because of 
// ERROR used in MessageType
#ifdef WIN32
#undef ERROR
#endif // WIN32


enum class MessageType
{
    INFO,
    WARNING,
    ERROR,
    NONE
};

struct Message
{
    std::string message;
    MessageType type;
};

typedef std::vector<Message> MessageStore;

// Appends newline and flushes
void logMsg(std::string&& msg);
void logMsg(std::string& msg);
void logWarning(std::string&& msg);
void logWarning(std::string& msg);
void logError(std::string&& msg);
void logError(std::string& msg);

// Doesn't append new line or flush
void logRaw(std::string&& msg, MessageType type);
void logRaw(std::string& msg, MessageType type);

MessageStore& getLogMessages();

// TODO: improve logging with spdlog

#endif //RAYTRACER_LOGGING_H