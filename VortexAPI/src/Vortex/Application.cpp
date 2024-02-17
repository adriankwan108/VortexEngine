#include "VortexPCH.hpp"
#include "Application.hpp"

namespace VX
{
    Application::Application()
    {
        VX_CORE_INFO("Application initiated...");
    }

    Application::~Application()
    {
        VX_CORE_INFO("Application closed...");
    }

    void Application::Run()
    {
        VX_CORE_INFO("Running application...");
        while(true)
        {
            
        }
    }
}
