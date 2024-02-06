#pragma once

#ifndef Application_hpp
#define Application_hpp

#include "Core.hpp"

#endif


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
