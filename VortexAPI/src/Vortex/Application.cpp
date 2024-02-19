#include "VortexPCH.hpp"
#include "Application.hpp"

#include "Events/ApplicationEvent.hpp"

namespace VX
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
        VX_CORE_INFO("Application initiated...");
    }

    Application::~Application()
    {
        VX_CORE_INFO("Application closed...");
    }

    void Application::Run()
    {
        VX_CORE_INFO("Running application...");

        while(m_Running)
        {
            m_Window->OnUpdate();
        }
    }
}
