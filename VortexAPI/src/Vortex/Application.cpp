//
//  EntryPoint.cpp
//  TemplateAPI
//
//  Created by Ting Fung Kwan on 25/1/2024.
//

#include "Application.hpp"
#include "Log.hpp"
#include <iostream>

namespace VX
{
    Application::Application()
    {
        VX_CORE_INFO("Application initiated...");
    }

    Application::~Application()
    {
        VX_CORE_INFO("Application closed...");
    }

    void Application::Run()
    {
        VX_CORE_INFO("Running application...");
        while(true)
        {
            
        }
    }
}
