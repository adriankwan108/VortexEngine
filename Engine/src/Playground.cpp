#include "Playground.hpp"

Playground::Playground()
    : VX::Layer("Playground")
{
    // define shader layout
    // create vertex array (layout)
    
    // simulate vertices data
    // create vertex buffer (data)
    
    // vertex array -> add vertex buffer
    
    // load shader
    
    VX_CORE_INFO("{0}: Created", GetName());
}

void Playground::OnAttach()
{
    VX_CORE_INFO("{0}: Attached", GetName());
}

void Playground::OnDetach()
{
    VX_CORE_INFO("{0}: Detached", GetName());
}

void Playground::OnUpdate()
{
    // update camera
    
    // renderer cmd
    // set clear color
    // begin scene
    
    // submit (shader, vertex array)
}

void Playground::OnEvent(VX::Event& event)
{
    
}

