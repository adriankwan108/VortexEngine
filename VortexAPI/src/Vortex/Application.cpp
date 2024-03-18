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

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
        // VX_CORE_TRACE("{0}", e);

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    void Application::Run()
    {
        VX_CORE_INFO("Running application...");

        while(m_Running)
        {
            if(!m_Minimized)
            {
                for (Layer* layer: m_LayerStack)
                {
                    layer->OnUpdate();
                }
            }
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent &event)
    {
        if(event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            m_Minimized = true;
            VX_CORE_INFO("Application: Window minimized...");
        }
        else
        {
            VX_CORE_INFO("Application: Window resized...");
            m_Minimized = false;
        }
        
        m_Window->Resize(event.GetWidth(), event.GetHeight());
        return false;
    }
}
