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

void logMsg(std::string&& msg);
void logMsg(std::string& msg);
void logWarning(std::string&& msg);
void logWarning(std::string& msg);
void logError(std::string&& msg);
void logError(std::string& msg);

const std::vector<std::string> logMessages();

#endif //RAYTRACER_LOGGING_H