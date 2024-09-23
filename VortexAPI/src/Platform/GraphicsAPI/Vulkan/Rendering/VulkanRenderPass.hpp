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
    class VulkanRenderPass;

    class VulkanRenderPassBuilder
    {
    public:
        explicit VulkanRenderPassBuilder(VkDevice device);
        ~VulkanRenderPassBuilder();
        
        VX::Scope<VulkanRenderPass> Create();
    private:
        VkDevice m_device;
        
        // subpass descriptions
        // subpass dependencies
        // attachments
    };

    // wrapper of VkRenderPass with RAII
    class VulkanRenderPass
    {
    public:
        VulkanRenderPass(VkDevice device);
        ~VulkanRenderPass();
        
    private:
        VkDevice m_device;
        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };

//    class VulkanRenderPass
//    {
//    public:
//        VulkanRenderPass(const std::string& name);
//        ~VulkanRenderPass();
//        
//        static void Init(VkDevice device);
//        
//        void AddSubpass(vkclass::VulkanSubpass subpass);
//        void Create();
//        
//        const std::string& Name = m_name;
//        const VkRenderPass& RenderPass = m_renderPass;
//        
//    private:
//        static VkDevice m_device;
//        
//        std::string m_name = "default";
//        
//        VkRenderPass m_renderPass;
//        
//        std::vector<VkSubpassDescription> m_subpasses;
//        std::vector<VkAttachmentDescription> m_attachments;
//        std::vector<VkSubpassDependency> m_dependencies;
//    };
//
//    class VulkanRenderPassManager
//    {
//    public:
//        VulkanRenderPassManager();
//        ~VulkanRenderPassManager();
//        
//        void Init(VkFormat mainFormat);
//        void AddRenderPass(VX::Scope<VulkanRenderPass>& renderPass);
//        
//        // TODO: generic reload; for now, this just destroy and rebuild renderpass & subpass
//        void Reload(const std::string& name, VkFormat format);
//
//        VkRenderPass GetRenderPass(const std::string& name);
//        
//    private:
//        std::map<std::string, VX::Scope<VulkanRenderPass>> m_renderPassMap;
//        
//    };
}
