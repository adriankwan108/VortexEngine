#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"

#include "VulkanSwapChain.hpp"
#include "VulkanDevice.hpp"
#include "VulkanTools.hpp"
#include "VulkanInitializer.hpp"
#include "VulkanSubpass.hpp"

namespace vkclass
{
    class VulkanRenderPass
    {
    public:
        VulkanRenderPass(VkDevice device);
        ~VulkanRenderPass();
        
        void AddSubpass(vkclass::VulkanSubpass subpass);
        void AddDependency();
        void Create();
        
        const VkRenderPass& RenderPass = m_renderPass;
        
    private:
        VkDevice m_device;
        
        std::string m_name = "default";
        
        VkRenderPass m_renderPass;
        
        std::vector<VkSubpassDescription> m_subpasses{};
        std::vector<VkAttachmentDescription> m_attachments{};
        std::vector<VkSubpassDependency> m_dependencies{};
    };
}
