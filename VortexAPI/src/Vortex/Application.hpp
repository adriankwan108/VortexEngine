#pragma once
#include "Core.hpp"
#include "Window.hpp"
#include "Events/ApplicationEvent.hpp"

namespace VX
{
    class VX_API Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
        
        void OnEvent(Event &e);
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        
        bool OnWindowClose(WindowCloseEvent& event);
    };

    // To be defined by client (global)
    Application* CreateApplication();
}
