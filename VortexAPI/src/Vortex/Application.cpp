#include "VortexPCH.hpp"
#include "Application.hpp"

#include "Events/ApplicationEvent.hpp"

namespace VX
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application::Application()
    {
        VX_CORE_INFO("Initiating application...");
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        VX_CORE_INFO("Application initiated.");
    }

    Application::~Application()
    {
        VX_CORE_INFO("Application closed...");
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        VX_CORE_TRACE("{0}", e);
    }

    void Application::Run()
    {
        VX_CORE_INFO("Running application...");

        while(m_Running)
        {
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_Running = false;
        return true;
    }
}
