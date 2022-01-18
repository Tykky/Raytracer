#ifndef RAYTRACER_LOGGING_H
#define RAYTRACER_LOGGING_H

#include <vector>
#include <string>

enum class MessageType
{
    INFO,
    WARNING,
    ERROR
};

struct Message
{
    std::string message;
    MessageType type;
};

typedef std::vector<Message> MessageStore;

void logMsg(const char* msg);
void logWarning(const char* msg);
void logError(const char* msg);
MessageStore* getLogMessages();

#endif //RAYTRACER_LOGGING_H