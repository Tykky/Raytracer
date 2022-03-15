#ifndef RAYTRACER_LOGGING_H
#define RAYTRACER_LOGGING_H

#include <vector>
#include <string>
#include <spdlog/spdlog.h>

#ifdef WIN32
#undef ERROR
#endif

void initLogger(const unsigned int ringSize);
spdlog::logger* logger();

#define RT_LOG_MSG(fmt, ...) logger()->info(fmt, __VA_ARGS__)
#define RT_LOG_WARNING(fmt, ...) logger()->warn(fmt, __VA_ARGS__)
#define RT_LOG_ERROR(fmt, ...) logger()->error(fmt, __VA_ARGS__)

const std::vector<std::string> logMessages();

#endif //RAYTRACER_LOGGING_H