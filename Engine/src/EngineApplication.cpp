#include "EngineApplication.hpp"
#include <iostream>
#include <Vortex.hpp>


class VortexEngine : public VX::Application
{
public:
    VortexEngine()
    {
        
    }
    
    ~VortexEngine()
    {
        
    }
};

VX::Application* VX::CreateApplication()
{
    return new VortexEngine();
}
