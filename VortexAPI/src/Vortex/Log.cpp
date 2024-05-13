#include "VortexPCH.hpp"
#include "Log.hpp"

namespace VX
{
    Ref<spdlog::logger> Log::s_CoreLogger;
    Ref<spdlog::logger> Log::s_ClientLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("VortexAPI");
        s_CoreLogger->set_level(spdlog::level::trace);
        
        s_ClientLogger = spdlog::stdout_color_mt("Engine");
        s_ClientLogger->set_level(spdlog::level::trace);

        VX_CORE_INFO("Log initiated...");
    }
}


