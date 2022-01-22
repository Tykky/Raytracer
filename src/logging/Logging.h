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

void logMsg (std::string&& msg, MessageStore& messageStore);
void logMsg (std::string&  msg);
void logWarning (std::string&& msg, MessageStore& messageStore);
void logWarning (std::string&  msg);
void logError (std::string&& msg, MessageStore& messageStore);
void logError (std::string&  msg);

// TODO: log message serialization

#endif //RAYTRACER_LOGGING_H