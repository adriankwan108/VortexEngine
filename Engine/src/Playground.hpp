#pragma once

#include <Vortex.hpp>

class Playground : public VX::Layer
{
public:

    Playground();
    ~Playground() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnEvent(VX::Event& event) override;
    
private:
    // prepare resources here
    
    // shader
    // vertex array
};
