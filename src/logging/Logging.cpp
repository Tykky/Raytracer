#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include "Logging.h"

static std::shared_ptr<spdlog::logger> LOGGER = nullptr;
static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> RINGBUFFER_SINK = nullptr;

static const char* formatting = "[%D %X] [%l] %v";

#ifdef WIN32
#include <spdlog/sinks/msvc_sink.h>
#include <windows.h>
#endif

const char* failedMsg = "Tried to use logger when it's not initialized! Call initlogger(int ringSize) first\n";

void initLogger(const unsigned int logSize)
{
    spdlog::set_pattern(formatting);
    LOGGER = spdlog::stdout_color_mt("raytracer");
    LOGGER->set_level(spdlog::level::trace);

    // in order to display log messages inside the editor
    RINGBUFFER_SINK = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(logSize);
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
    if (!LOGGER) 
    {
        fprintf(stderr, failedMsg);
#ifdef WIN32
        OutputDebugString(failedMsg);
#endif
    }
    return LOGGER.get();
}

const std::vector<std::string> logMessages()
{
    if (!RINGBUFFER_SINK) 
    {
        fprintf(stderr, failedMsg);
#ifdef WIN32
        OutputDebugString(failedMsg);
#endif
    }
    return RINGBUFFER_SINK->last_formatted();
}
