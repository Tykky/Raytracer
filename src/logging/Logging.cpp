#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include "Logging.h"

static std::shared_ptr<spdlog::logger> LOGGER;
static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> RINGBUFFER_SINK;

static const char* formatting = "%^[&T] %n: %v%$";

#ifdef WIN32
#include <spdlog/sinks/msvc_sink.h>
#endif

void initLogger()
{
    spdlog::set_pattern(formatting);
    LOGGER = spdlog::stdout_color_mt("raytracer");
    LOGGER->set_level(spdlog::level::trace);

    constexpr int ringSize = 128;

    // in order to display log messages inside the editor
    RINGBUFFER_SINK = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(ringSize);

    LOGGER->sinks().push_back(RINGBUFFER_SINK);

#ifdef WIN32
    auto msvc = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    LOGGER->sinks().push_back(msvc);
#endif
}

void logMsg(std::string&& msg)
{
    LOGGER->info(std::move(msg));
}

void logMsg(std::string& msg)
{
    LOGGER->info(msg);
}

void logWarning(std::string&& msg)
{
    LOGGER->warn(std::move(msg));
}

void logWarning(std::string& msg)
{
    LOGGER->warn(msg);
}

void logError(std::string&& msg)
{
    LOGGER->error(std::move(msg));
}

void logError(std::string& msg)
{
    LOGGER->error(msg);
}

const std::vector<std::string> logMessages()
{
    return RINGBUFFER_SINK->last_formatted();
}
