#pragma once
#include "Core.hpp"
#include "Window.hpp"

namespace VX
{
    class VX_API Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
    private:
        std::unique_ptr<Window> m_Window;
    };

    // To be defined by client (global)
    Application* CreateApplication();
}
