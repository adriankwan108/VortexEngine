#include "VortexPCH.hpp"
#include "Application.hpp"

#include "Events/ApplicationEvent.hpp"

namespace VX
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application::Application()
    {
        VX_CORE_INFO("Application: Initiating...");
        
        VX_CORE_INFO("Application: Creating window...");
        m_Window = std::unique_ptr<Window>(Window::Create());
        
        VX_CORE_INFO("Application: Binding events...");
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        
        VX_CORE_INFO("Application: preparing renderer...");
        Renderer::Init();
        
        // layer of gui
        
        VX_CORE_INFO("Application initiated.");
    }

    Application::~Application()
    {
        VX_CORE_INFO("Application closing...");
        m_Window->OnClose(); // this is not inside OnWindowClose, as this is outside game loop
        Renderer::ShutDown();
        VX_CORE_INFO("Application closed.");
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
            m_Window->OnUpdateStart();
            if(!m_Minimized)
            {
                for (Layer* layer: m_LayerStack)
                {
                    layer->OnUpdate(); // playground: update
                }
            }
            m_Window->OnUpdate(); // 
            m_Window->OnUpdateEnd();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_Running = false;
        // below would be inside the last update of graphics api display loop
        
        // log anything needed
        // confirm closed ?
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
