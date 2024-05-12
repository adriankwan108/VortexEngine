#pragma once
#include "Core.hpp"
#include "Window.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Renderer/Renderer.hpp"
#include "Core/Timestep.hpp"
#include "UI/UILayer.hpp"

namespace VX
{
    class VX_API Application
    {
    public:
        Application();
        ~Application();
        
        void Run(); // private ?
        
        void OnEvent(Event &e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        
        Window& GetWindow() { return *m_Window;}
        UILayer* GetUILayer() { return m_uiLayer; }
        
        static Application& Get() { return *s_Instance; }
    private:
        // status
        bool m_Running = true;
        bool m_Minimized = false;
        
        // props
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;

        UILayer* m_uiLayer;

        float m_LastFrameTime = 0.0f;
        
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);
        
    private:
        static Application* s_Instance;
    };

    // To be defined by client (global)
    Application* CreateApplication();
}
