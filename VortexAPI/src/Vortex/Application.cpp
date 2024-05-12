#include "VortexPCH.hpp"
#include "Application.hpp"

#include "Events/ApplicationEvent.hpp"
#include <GLFW/glfw3.h>

namespace VX
{

    Application* Application::s_Instance = nullptr;

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application::Application()
    {
        VX_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        
        VX_CORE_INFO("Application: Initiating...");
        
        VX_CORE_INFO("Application: Creating window...");
        m_Window = std::unique_ptr<Window>(Window::Create());
        
        VX_CORE_INFO("Application: Setting event callbacks...");
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        
        VX_CORE_INFO("Application: preparing renderer...");
        Renderer::Init();
        
        VX_CORE_INFO("Application: preparing ui layer...");
        m_uiLayer = UILayer::Create();
        PushOverlay(m_uiLayer);
        
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
            float time = (float)glfwGetTime(); // TODO: platform: get time
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;
            
            m_Window->OnUpdateStart();
            if(!m_Minimized)
            {
                for (Layer* layer: m_LayerStack)
                {
                    layer->OnUpdate(timestep); // playground: update
                }

                m_uiLayer->OnUpdateStart();
                for (Layer* layer : m_LayerStack)
                {
                    // layer->OnUIRender();
                }
                m_uiLayer->OnUpdateEnd();
            }
            m_Window->OnUpdate(); // reserved

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
