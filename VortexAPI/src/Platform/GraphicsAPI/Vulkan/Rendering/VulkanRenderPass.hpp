#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <map>

#include "Renderer/RenderTarget.hpp"
#include "Core/VulkanDevice.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanInitializer.hpp"
#include "VulkanSwapChain.hpp"

namespace vkclass
{
    /*
     This contains all Vulkan attachments' descriptions and creates references,
     which descriptions are gathered from render target specification, or by raw
     */
    class VulkanRenderPassSpecification
    {
    public:
        VulkanRenderPassSpecification(const VX::RenderTargetSpecification& rtSpec);
        VulkanRenderPassSpecification(VX::SubpassHint hint, std::vector<VkAttachmentDescription> colorAttachmentDesciptions);
        // get hash
        
        inline const VX::SubpassHint& GetSubpassHint() { return m_subpassHint; }
        
        inline uint32_t GetColorAttachmentsNum() const { return m_colorAttachmentNum; }
        inline const VkAttachmentReference* GetColorAttachmentReferences() const { return m_colorAttachmentNum > 0 ? m_colorReferences : nullptr; }
        
        inline uint32_t GetTotalAttachmentNum() const { return m_TotalAttachmentNum; }
        inline const VkAttachmentDescription* GetAttachmentDescription() const { return m_desc; }
        
    private:
        static const int MaxSimultaneousRenderTargets = 1024; // TODO: should be defined somewhere else
        
        VX::SubpassHint m_subpassHint = VX::SubpassHint::None;
        
        uint32_t m_TotalAttachmentNum;
        uint8_t m_colorAttachmentNum;
        VkAttachmentReference m_colorReferences[MaxSimultaneousRenderTargets];
        
        // Depth in "+1", Shading Rate texture in "+2"
        // VkAttachmentDescription Desc[MaxSimultaneousRenderTargets * 2 + 2];
        VkAttachmentDescription m_desc[MaxSimultaneousRenderTargets * 2];
    };

    /*
     This builds the subpasses details from subpass hint and render pass specification
     */
    class VulkanRenderPassBuilder
    {
    public:
        explicit VulkanRenderPassBuilder(VkDevice device);
        ~VulkanRenderPassBuilder();
        
        void Create(const VulkanRenderPassSpecification& spec, VkRenderPass* renderPass);
    private:
        VkDevice m_device;
        
        VkSubpassDescription m_SubpassDescriptions[8];
        VkSubpassDependency m_SubpassDependencies[8];
        
        std::vector<VkAttachmentDescription> m_attachmentDescriptions;
        std::vector<VkAttachmentReference> m_colorAttachmentReferences;
        
        VkRenderPassCreateInfo m_renderPassCreateInfo{};
    };

    // wrapper of VkRenderPass with RAII
    class VulkanRenderPass
    {
    public:
        VulkanRenderPass(VkDevice device, const VulkanRenderPassSpecification& spec);
        ~VulkanRenderPass();
        
    private:
        VkDevice m_device;
        VulkanRenderPassSpecification m_spec;
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
