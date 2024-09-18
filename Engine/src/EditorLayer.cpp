#include "EditorLayer.hpp"

EditorLayer::EditorLayer()
{
    
}

void EditorLayer::OnAttach()
{
    VX_INFO("Editor::OnAttached.");
    VX::RenderTargetSpecification spec;
    spec.IsSwapChainTarget = true;
    m_EditorFrame = VX::RenderTarget::Create(spec);
}

void EditorLayer::OnDetach()
{
    VX_INFO("Editor::OnDetached.");
}

void EditorLayer::OnUpdate(VX::Timestep ts)
{
    
}

void EditorLayer::OnEvent(VX::Event &event)
{
    
}
