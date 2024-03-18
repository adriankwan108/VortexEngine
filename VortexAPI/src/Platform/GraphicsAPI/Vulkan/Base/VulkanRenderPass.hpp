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
        VulkanRenderPass();
        ~VulkanRenderPass();
        
        static void Init(VkDevice device);
        
        void AddSubpass(vkclass::VulkanSubpass& subpass);
        void AddDependency();
        void Create();
        
        const VkRenderPass& RenderPass = m_renderPass;
        
    private:
        static VkDevice m_device;
        
        std::string m_name = "default";
        
        VkRenderPass m_renderPass;
        
        std::vector<VkSubpassDescription> m_subpasses{};
        std::vector<VkAttachmentDescription> m_attachments{};
        std::vector<VkSubpassDependency> m_dependencies{};
    };
}
