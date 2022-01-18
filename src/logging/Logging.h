#ifndef RAYTRACER_LOGGING_H
#define RAYTRACER_LOGGING_H

enum class MessageType
{
    INFO,
    WARNING,
    ERROR
};

struct Message
{
    const char* message;
    MessageType type;
};


void logMsg(const char* msg);
void logWarning(const char* msg);
void logError(const char* msg);

#endif //RAYTRACER_LOGGING_H