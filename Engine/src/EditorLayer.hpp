#pragma once

#include <Vortex.hpp>

class EditorLayer : public VX::Layer
{
public:
    EditorLayer();
    ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(VX::Timestep ts) override;
    virtual void OnEvent(VX::Event& event) override;
    
private:

};
