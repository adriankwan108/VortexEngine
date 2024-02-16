#pragma once

#include "Core.hpp"
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> 
#include "spdlog/sinks/stdout_color_sinks.h"

namespace VX
{
    class VX_API Log
    {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define VX_CORE_TRACE(...)    ::VX::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VX_CORE_INFO(...)     ::VX::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VX_CORE_WARN(...)     ::VX::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VX_CORE_ERROR(...)    ::VX::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VX_CORE_CRITICAL(...) ::VX::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define VX_TRACE(...)         ::VX::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VX_INFO(...)          ::VX::Log::GetClientLogger()->info(__VA_ARGS__)
#define VX_WARN(...)          ::VX::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VX_ERROR(...)         ::VX::Log::GetClientLogger()->error(__VA_ARGS__)
#define VX_CRITICAL(...)      ::VX::Log::GetClientLogger()->critical(__VA_ARGS__)
