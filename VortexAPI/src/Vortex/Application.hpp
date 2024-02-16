#pragma once
#include "Core.hpp"

namespace VX
{
    class VX_API Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
    };

    // To be defined by client (global)
    Application* CreateApplication();
}
