#include "EngineApplication.hpp"
#include <iostream>
#include <Template.hpp>


class EngineApplication : public VX::Application
{
public:
    EngineApplication()
    {
        
    }
    
    ~EngineApplication()
    {
        
    }
};

VX::Application* VX::CreateApplication()
{
    return new EngineApplication();
}
