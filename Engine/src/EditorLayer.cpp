#include "EditorLayer.hpp"

EditorLayer::EditorLayer()
{
    
}

void EditorLayer::OnAttach()
{
    VX_INFO("Editor::OnAttached.");
    VX::RenderTargetSpecification spec;
    spec.IsSwapChainTarget = true;
    m_EditorFrame = VX::RenderTarget::Create("editor", spec);
}

void EditorLayer::OnDetach()
{
    VX_INFO("Editor::OnDetached.");
}

void EditorLayer::OnUpdate(VX::Timestep ts)
{
    m_EditorFrame->Bind();
}

void EditorLayer::OnEvent(VX::Event &event)
{
    
}
