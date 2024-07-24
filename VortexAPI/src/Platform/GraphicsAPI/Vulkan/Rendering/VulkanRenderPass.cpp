#include "VulkanRenderPass.hpp"

namespace vkclass
{
    VkDevice vkclass::VulkanRenderPass::m_device = VK_NULL_HANDLE;

    VulkanRenderPass::VulkanRenderPass(const std::string& name)
        :m_name(name)
    {
        
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
        VX_CORE_INFO("VulkanRenderPass: PipelineLayout destroyed");
        VX_CORE_INFO("VulkanRenderPass: RenderPass destroyed");
    }

    void VulkanRenderPass::Init(VkDevice device)
    {
        m_device = device;
    }

    void VulkanRenderPass::AddSubpass(vkclass::VulkanSubpass subpass)
    {
        m_subpasses.push_back(subpass.Subpass);
        
        VX_CORE_TRACE("RenderPass: Main attachment count before: {0}", m_attachments.size());
        if(m_attachments.empty())
        {
            m_attachments = subpass.GetAttachments();
        }else{
            m_attachments.insert(m_attachments.end(), subpass.GetAttachments().begin(), subpass.GetAttachments().end());
        }
        VX_CORE_TRACE("RenderPass: Main attachments count after: {0}", m_attachments.size());
        
        VX_CORE_TRACE("RenderPass: Main dependencies count before: {0}", m_dependencies.size());
        if(m_dependencies.empty())
        {
            m_dependencies = subpass.GetDependencies();
        }else{
            m_dependencies.insert(m_dependencies.end(), subpass.GetDependencies().begin(), subpass.GetDependencies().end());
        }
        VX_CORE_TRACE("RenderPass: Main dependencies count after: {0}", m_dependencies.size());
    }

    void VulkanRenderPass::Create()
    {
        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        // attachments
        createInfo.attachmentCount = static_cast<uint32_t>(m_attachments.size());
        createInfo.pAttachments = m_attachments.data();
        VX_CORE_INFO("Render Pass: attachment count: {0}", m_attachments.size());
        // subpasses
        createInfo.subpassCount = static_cast<uint32_t>(m_subpasses.size());
        createInfo.pSubpasses = m_subpasses.data();
        
        createInfo.dependencyCount = static_cast<uint32_t>(m_dependencies.size());
        createInfo.pDependencies = m_dependencies.data();
        
        VK_CHECK_RESULT(vkCreateRenderPass(m_device, &createInfo, nullptr, &m_renderPass));
    }


    VulkanRenderPassManager::VulkanRenderPassManager()
    {

    }

    void VulkanRenderPassManager::AddRenderPass(VX::Ref<VulkanRenderPass> renderPass)
    {
        // note that this will overwrite the current one
        m_renderPassMap[renderPass->Name] = renderPass->RenderPass;
    }

    void VulkanRenderPassManager::Reload()
    {
        
    }

    VkRenderPass VulkanRenderPassManager::GetRenderPass(const std::string & name)
    {
        auto it = m_renderPassMap.find(name);
        if(it != m_renderPassMap.end())
        {
            return it->second;
        }else
        {
            return nullptr;
        }
    }
}
