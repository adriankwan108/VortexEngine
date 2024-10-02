#include "VulkanRenderPass.hpp"

namespace vkclass
{

    VulkanRenderPassBuilder::VulkanRenderPassBuilder(VkDevice device)
        :m_device(device)
    {
        
    }

    VulkanRenderPassBuilder::~VulkanRenderPassBuilder()
    {
    }

    void VulkanRenderPassBuilder::Create(const VulkanRenderPassSpecification& spec, VkRenderPass* renderPass)
    {
        if(renderPass != nullptr && *renderPass != VK_NULL_HANDLE)
        {
            VX_CORE_TRACE("RenderPassBuilder:: Destroy input renderpass...");
        }
        
        VX_CORE_TRACE("RenderPassBuilder:: Creating...");
        uint32_t currSubpassIndex     = 0;
        uint32_t currDependencyIndex  = 0;

        // configs:
        // isDeferredShading = info.GetSubpassHint() == Deferred
        // isDepthReadSubpass
        // hasDepthAttachmentRef

        // grab all attachment descriptions from info

        // grab color attachment references from info

        // grab depth stencil attachment references from info

        /* create subpass */
        // main subpass
        {
            VkSubpassDescription& subpassDesc = m_SubpassDescriptions[currSubpassIndex++];
            subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDesc.colorAttachmentCount = static_cast<uint32_t>(m_colorAttachmentReferences.size());
            subpassDesc.pColorAttachments = m_colorAttachmentReferences.data();

            // depth
            // shading rate
        }

        // color write, depth read subpass
        {
            // subpass dependency
        }

        // two subpasses for deferred shading
        {
            // 1. Write: scene, gbuffer ; Input: depth
            // 2. Write: scene          ; Input: gbuffer, depth
        }

        // custom resolve subpass
        {

        }

        // create render pass
        m_renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        // total attachment
//        m_renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(m_attachmentDescriptions.size());
//        m_renderPassCreateInfo.pAttachments = m_attachmentDescriptions.data();
//
//        m_renderPassCreateInfo.subpassCount = currSubpassIndex;
//        m_renderPassCreateInfo.pSubpasses = m_SubpassDescriptions;
//        m_renderPassCreateInfo.dependencyCount = currDependencyIndex;
//        m_renderPassCreateInfo.pDependencies = m_SubpassDependencies;
        
//        VK_CHECK_RESULT(vkCreateRenderPass(m_device, &m_renderPassCreateInfo, nullptr, renderPass));
    }


    VulkanRenderPass::VulkanRenderPass(VkDevice device, const VulkanRenderPassSpecification& spec)
        :m_device(device), m_spec(spec)
    {
        VulkanRenderPassBuilder builder(m_device);
        builder.Create(spec, &m_RenderPass);
        VX_CORE_TRACE("RenderPass created.");
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        if(m_device != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(m_device, m_RenderPass, nullptr);
        }
        VX_CORE_TRACE("RenderPass destroyed.");
    }

//    VkDevice vkclass::VulkanRenderPass::m_device = VK_NULL_HANDLE;
//
//    VulkanRenderPass::VulkanRenderPass(const std::string& name)
//        :m_name(name)
//    {
//
//    }
//
//    VulkanRenderPass::~VulkanRenderPass()
//    {
//        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
//        VX_CORE_INFO("VulkanRenderPass: PipelineLayout destroyed");
//        VX_CORE_INFO("VulkanRenderPass: RenderPass destroyed");
//    }
//
//    void VulkanRenderPass::Init(VkDevice device)
//    {
//        m_device = device;
//    }
//
//    void VulkanRenderPass::AddSubpass(vkclass::VulkanSubpass subpass)
//    {
//        m_subpasses.push_back(subpass.Subpass);
//
//        if(m_attachments.empty())
//        {
//            m_attachments = subpass.GetAttachments();
//        }else
//        {
//            m_attachments.insert(m_attachments.end(), subpass.GetAttachments().begin(), subpass.GetAttachments().end());
//        }
//
//        if(m_dependencies.empty())
//        {
//            m_dependencies = subpass.GetDependencies();
//        }else
//        {
//            m_dependencies.insert(m_dependencies.end(), subpass.GetDependencies().begin(), subpass.GetDependencies().end());
//        }
//    }
//
//    void VulkanRenderPass::Create()
//    {
//        VkRenderPassCreateInfo createInfo{};
//        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//        // attachments
//        createInfo.attachmentCount = static_cast<uint32_t>(m_attachments.size());
//        createInfo.pAttachments = m_attachments.data();
//        // subpasses
//        createInfo.subpassCount = static_cast<uint32_t>(m_subpasses.size());
//        createInfo.pSubpasses = m_subpasses.data();
//
//        createInfo.dependencyCount = static_cast<uint32_t>(m_dependencies.size());
//        createInfo.pDependencies = m_dependencies.data();
//
//        VK_CHECK_RESULT(vkCreateRenderPass(m_device, &createInfo, nullptr, &m_renderPass));
//        VX_CORE_INFO("VulkanRenderPass: \"{0}\" created", m_name);
//    }
//
//
//    VulkanRenderPassManager::VulkanRenderPassManager()
//    {
//
//    }
//
//    VulkanRenderPassManager::~VulkanRenderPassManager()
//    {
//        m_renderPassMap.clear();
//    }
//
//    void VulkanRenderPassManager::Init(VkFormat mainFormat)
//    {
//        // create main render pass
//        auto mainRenderPass = VX::CreateScope<vkclass::VulkanRenderPass>("main");
//        vkclass::VulkanSubpass GeometrySubpass(
//            vkclass::SubpassType::main, mainFormat
//        );
//        mainRenderPass->AddSubpass(GeometrySubpass);
//        mainRenderPass->Create();
//
//        AddRenderPass(mainRenderPass);
//    }
//
//    void VulkanRenderPassManager::AddRenderPass(VX::Scope<VulkanRenderPass>& renderPass)
//    {
//        // note that this will overwrite the current one
//        m_renderPassMap[renderPass->Name] = std::move(renderPass);
//    }
//
//    void VulkanRenderPassManager::Reload(const std::string &name, VkFormat format)
//    {
//        // delete
//        auto it = m_renderPassMap.find(name);
//        if(it != m_renderPassMap.end())
//        {
//            m_renderPassMap.erase(it);
//            // create main render pass
//            auto mainRenderPass = VX::CreateScope<vkclass::VulkanRenderPass>("main");
//            vkclass::VulkanSubpass GeometrySubpass(
//                vkclass::SubpassType::main, format
//            );
//            mainRenderPass->AddSubpass(GeometrySubpass);
//            mainRenderPass->Create();
//            AddRenderPass(mainRenderPass);
//        }else
//        {
//            VX_CORE_WARN("VulkanRenderPassManager::Reload: name not found.");
//        }
//    }
//
//    VkRenderPass VulkanRenderPassManager::GetRenderPass(const std::string& name)
//    {
//        auto it = m_renderPassMap.find(name);
//        if(it != m_renderPassMap.end())
//        {
//            return it->second->RenderPass;
//        }else
//        {
//            return VK_NULL_HANDLE;
//        }
//    }
}
