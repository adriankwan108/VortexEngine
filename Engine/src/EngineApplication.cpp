#include <Vortex.hpp>
#include <Vortex/EntryPoint.hpp>

#include "Playground.hpp"
#include "EditorLayer.hpp"

class VortexEngine : public VX::Application
{
public:
    VortexEngine()
    {
         // PushLayer(new Playground());
        PushLayer(new EditorLayer());
    }
    
    ~VortexEngine()
    {
        
    }
};

VX::Application* VX::CreateApplication()
{
    return new VortexEngine();
}
