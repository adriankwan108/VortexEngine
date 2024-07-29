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
        
        if(m_attachments.empty())
        {
            m_attachments = subpass.GetAttachments();
        }else
        {
            m_attachments.insert(m_attachments.end(), subpass.GetAttachments().begin(), subpass.GetAttachments().end());
        }

        if(m_dependencies.empty())
        {
            m_dependencies = subpass.GetDependencies();
        }else
        {
            m_dependencies.insert(m_dependencies.end(), subpass.GetDependencies().begin(), subpass.GetDependencies().end());
        }
    }

    void VulkanRenderPass::Create()
    {
        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        // attachments
        createInfo.attachmentCount = static_cast<uint32_t>(m_attachments.size());
        createInfo.pAttachments = m_attachments.data();
        // subpasses
        createInfo.subpassCount = static_cast<uint32_t>(m_subpasses.size());
        createInfo.pSubpasses = m_subpasses.data();
        
        createInfo.dependencyCount = static_cast<uint32_t>(m_dependencies.size());
        createInfo.pDependencies = m_dependencies.data();
        
        VK_CHECK_RESULT(vkCreateRenderPass(m_device, &createInfo, nullptr, &m_renderPass));
        VX_CORE_INFO("VulkanRenderPass: \"{0}\" created", m_name);
    }


    VulkanRenderPassManager::VulkanRenderPassManager()
    {

    }

    VulkanRenderPassManager::~VulkanRenderPassManager()
    {
        m_renderPassMap.clear();
    }

    void VulkanRenderPassManager::Init(VkFormat mainFormat)
    {
        // create main render pass
        auto mainRenderPass = VX::CreateScope<vkclass::VulkanRenderPass>("main");
        vkclass::VulkanSubpass GeometrySubpass(
            vkclass::SubpassType::main, mainFormat
        );
        mainRenderPass->AddSubpass(GeometrySubpass);
        mainRenderPass->Create();
        
        AddRenderPass(mainRenderPass);
    }

    void VulkanRenderPassManager::AddRenderPass(VX::Scope<VulkanRenderPass>& renderPass)
    {
        // note that this will overwrite the current one
        m_renderPassMap[renderPass->Name] = std::move(renderPass);
    }

    void VulkanRenderPassManager::Reload(const std::string &name, VkFormat format)
    {
        // delete
        auto it = m_renderPassMap.find(name);
        if(it != m_renderPassMap.end())
        {
            m_renderPassMap.erase(it);
            // create main render pass
            auto mainRenderPass = VX::CreateScope<vkclass::VulkanRenderPass>("main");
            vkclass::VulkanSubpass GeometrySubpass(
                vkclass::SubpassType::main, format
            );
            mainRenderPass->AddSubpass(GeometrySubpass);
            mainRenderPass->Create();
            AddRenderPass(mainRenderPass);
        }else
        {
            VX_CORE_WARN("VulkanRenderPassManager::Reload: name not found.");
        }
    }

    VkRenderPass VulkanRenderPassManager::GetRenderPass(const std::string& name)
    {
        auto it = m_renderPassMap.find(name);
        if(it != m_renderPassMap.end())
        {
            return it->second->RenderPass;
        }else
        {
            return VK_NULL_HANDLE;
        }
    }
}
