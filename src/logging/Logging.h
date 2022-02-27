#ifndef RAYTRACER_LOGGING_H
#define RAYTRACER_LOGGING_H

#include <vector>
#include <string>

#ifdef WIN32
#undef ERROR
#endif

// Cursed ms macro. Needs to be undefined since ERROR is used in MessageType
#ifdef WIN32
#undef ERROR
#endif // WIN32

void initLogger();

// Appends newline and flushes
void logMsg(std::string&& msg);
void logMsg(std::string& msg);
void logWarning(std::string&& msg);
void logWarning(std::string& msg);
void logError(std::string&& msg);
void logError(std::string& msg);

// Doesn't append new line or flush
void logRaw(std::string&& msg);
void logRaw(std::string& msg);

const std::vector<std::string> logMessages();

// TODO: improve logging with spdlog

#endif //RAYTRACER_LOGGING_H