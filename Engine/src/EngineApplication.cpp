#include <Vortex.hpp>
#include <Vortex/EntryPoint.hpp>

#include "Playground.hpp"

class VortexEngine : public VX::Application
{
public:
    VortexEngine()
    {
         // PushLayer(new Playground());
    }
    
    ~VortexEngine()
    {
        
    }
};

VX::Application* VX::CreateApplication()
{
    return new VortexEngine();
}
