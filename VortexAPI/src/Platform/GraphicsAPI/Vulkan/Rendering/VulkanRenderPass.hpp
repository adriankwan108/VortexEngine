#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <map>

#include "Core/VulkanDevice.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanInitializer.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanSubpass.hpp"

namespace vkclass
{
    class VulkanRenderPass
    {
    public:
        VulkanRenderPass(const std::string& name);
        ~VulkanRenderPass();
        
        static void Init(VkDevice device);
        
        void AddSubpass(vkclass::VulkanSubpass subpass);
        void Create();
        
        const std::string& Name = m_name;
        const VkRenderPass& RenderPass = m_renderPass;
        
    private:
        static VkDevice m_device;
        
        std::string m_name = "default";
        
        VkRenderPass m_renderPass;
        
        std::vector<VkSubpassDescription> m_subpasses;
        std::vector<VkAttachmentDescription> m_attachments;
        std::vector<VkSubpassDependency> m_dependencies;
    };

    class VulkanRenderPassManager
    {
    public:
        VulkanRenderPassManager();
        ~VulkanRenderPassManager() = default;
        
        void AddRenderPass(VX::Ref<VulkanRenderPass> renderPass);
        void Reload();
        VkRenderPass GetRenderPass(const std::string& name);
        
    private:
        std::map<std::string, VkRenderPass> m_renderPassMap;
        // createVulkanRenderPass();
    };
}
