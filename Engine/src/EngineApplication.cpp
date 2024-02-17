#include "EngineApplication.hpp"
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
