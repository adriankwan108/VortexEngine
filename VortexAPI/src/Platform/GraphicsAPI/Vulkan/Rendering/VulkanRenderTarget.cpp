#include "VortexPCH.hpp"
#include "VulkanRenderTarget.hpp"

namespace vkclass
{
    VulkanRenderTarget::VulkanRenderTarget(const VX::RenderTargetSpecification& spec)
    {

    }

    VulkanRenderTarget::~VulkanRenderTarget()
    {

    }

    void VulkanRenderTarget::Init(VulkanRenderTargetManager* manager)
    {
        s_manager = manager;
    }

    void VulkanRenderTarget::Bind()
    {

    }

    void VulkanRenderTarget::Unbind()
    {

    }
}