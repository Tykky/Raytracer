#include <iostream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include "Logging.h"

static std::shared_ptr<spdlog::logger> LOGGER;
static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> RINGBUFFER_SINK;

static const char* formatting = "[%D %X] [%l] %v";

#ifdef WIN32
#include <spdlog/sinks/msvc_sink.h>
#endif

void initLogger(const unsigned int ringSize)
{
    spdlog::set_pattern(formatting);
    LOGGER = spdlog::stdout_color_mt("raytracer");
    LOGGER->set_level(spdlog::level::trace);

    // in order to display log messages inside the editor
    RINGBUFFER_SINK = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(ringSize);
    RINGBUFFER_SINK->set_pattern(formatting);

    LOGGER->sinks().push_back(RINGBUFFER_SINK);

#ifdef WIN32
    auto msvc = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    msvc->set_pattern(formatting);
    LOGGER->sinks().push_back(msvc);
#endif
}

spdlog::logger* logger() 
{
    return LOGGER.get();
}

const std::vector<std::string> logMessages()
{
    return RINGBUFFER_SINK->last_formatted();
}
