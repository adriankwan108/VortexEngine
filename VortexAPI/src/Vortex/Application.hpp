#pragma once
#include "Core.hpp"
#include "Window.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"

namespace VX
{
    class VX_API Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
        
        void OnEvent(Event &e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        
        bool OnWindowClose(WindowCloseEvent& event);
    };

    // To be defined by client (global)
    Application* CreateApplication();
}
