#include "VulkanUILayer.hpp"
#include "Application.hpp"

namespace vkclass
{
    VulkanUILayer::VulkanUILayer()
        : VX::UILayer()
    {
    }

    void VulkanUILayer::OnAttach()
    {
        VX_CORE_INFO("VulkanUILayer: OnAttach");
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        
        // configuration flags
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        // setup style
        ImGui::StyleColorsDark();
        
        // load fonts
        // io.FontGlobalScale
        
        // dimensions
        auto width = static_cast<float>(VX::Application::Get().GetWindow().GetWidth());
        auto height = static_cast<float>(VX::Application::Get().GetWindow().GetHeight());
        io.DisplaySize = ImVec2(width, height);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        
        // key mapping
        
        // link to our vulkan renderer
        
        // init necessary resources (if any ?)
    }

    void VulkanUILayer::OnDetach()
    {
        VX_CORE_INFO("VulkanUILayer: OnDetach");
        ImGui::DestroyContext();
    }

    void VulkanUILayer::OnEvent(VX::Event& event)
    {
        // input polling
        
        // resize event
    }
    

    void VulkanUILayer::OnUpdateStart()
    {
        ImGui::NewFrame();
    }

    void VulkanUILayer::OnUpdateEnd()
    {
        ImGui::Render();
    }
}
