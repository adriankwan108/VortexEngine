#pragma once
#include "Core.hpp"
#include "Window.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Renderer/Renderer.hpp"

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
        // status
        bool m_Running = true;
        bool m_Minimized = false;
        
        // props
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;
        
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);
    };

    // To be defined by client (global)
    Application* CreateApplication();
}
