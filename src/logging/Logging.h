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

void logMsg (std::string&& msg);
void logWarning (std::string&& msg);
void logError (std::string&& msg);

const Message* getLogMessages(int& size);

// TODO: log message serialization, spdlog

#endif //RAYTRACER_LOGGING_H