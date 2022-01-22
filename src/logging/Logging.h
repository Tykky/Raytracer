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

void logMsg(const std::string& msg, MessageStore& messageStore);
void logWarning(const std::string& msg, MessageStore& messageStore);
void logError(const std::string& msg, MessageStore& messageStore);

// TODO: log message serialization

#endif //RAYTRACER_LOGGING_H