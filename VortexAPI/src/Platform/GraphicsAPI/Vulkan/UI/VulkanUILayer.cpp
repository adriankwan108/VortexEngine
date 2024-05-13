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
    }

    void VulkanUILayer::OnDetach()
    {
        VX_CORE_INFO("VulkanUILayer: OnDetach");
        ImGui::DestroyContext();
    }

    void VulkanUILayer::OnEvent(VX::Event& event)
    {
    }
    

    void VulkanUILayer::OnUpdateStart()
    {
    }

    void VulkanUILayer::OnUpdateEnd()
    {
    }
}
