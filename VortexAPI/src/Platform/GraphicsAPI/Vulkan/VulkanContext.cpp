#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window):
        m_Window(window),
        m_Instance(m_enableValidation),
        m_Surface(window, &m_Instance),
        m_Device(&m_Instance, &m_Surface, m_enableValidation),
        m_SyncManager(&m_Device, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame),
        m_CommandManager(&m_Device, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame),
        m_DescriptorManager(&m_Device, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame)
    {

    }

    VulkanContext::~VulkanContext()
    {
        for(auto framebuffer: m_FrameBuffers)
        {
            delete framebuffer;
        }
        
        if(m_RenderPass != nullptr)
        {
            delete  m_RenderPass;
        }
        
        if(m_SwapChain != nullptr)
        {
            delete m_SwapChain;
        }

        vkclass::GlobalDescriptor::Remove();

        VX_CORE_INFO("Vulkan: Resources cleared.");
        VX_CORE_INFO("Vulkan: Destroying context...");
    }

    void VulkanContext::Init()
    {
        VX_CORE_INFO("Initiating Vulkan resources...");
        
        vkclass::DescriptorManager::Init(&m_DescriptorManager);
        
        vkclass::VulkanSwapChain::Init(&m_Device);
        vkclass::VulkanRenderPass::Init(m_Device.LogicalDevice);

        vkclass::VulkanPipelineBuilder::Init(m_Device.LogicalDevice);
        vkclass::VulkanBuffer::Init(&m_Device, &m_CommandManager);
        
        m_SwapChain = new vkclass::VulkanSwapChain(&m_Surface);
        m_SwapChain->CreateSwapChain();
        
        m_RenderPass = new vkclass::VulkanRenderPass();
        createRenderPass();
        
        vkclass::VulkanRendererAPI::SetCommandManager(&m_CommandManager);
        
        vkclass::VulkanCamera::Init(&m_Device);
        vkclass::VulkanShader::Init(m_Device.LogicalDevice, &m_CommandManager, m_RenderPass->RenderPass);
        
        createFrameBuffers();
        
    }

    void VulkanContext::DisplayStart()
    {
        // all operations in display are asynchronous, inflight frames enabled
        // sync manager operates objects with current rendering frame (i.e. fence, semaphores)
        // cmd manager operates objects with current rendering frame (e.g. current rendering cmd buffer)
        
        m_SyncManager.WaitForFences();
        m_DescriptorManager.Reset();
        m_acquireNextImageResult = m_SwapChain->AcquireNextImage(m_SyncManager.GetImageAvailableSemaphore());
        if( m_acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) // btw the return state is not guranteed by drivers / platforms
        {
            VX_CORE_INFO("Vulkan Context: Display() acquire image out of date, resizing...");
            resizeHelper();
            return;
        }else if(m_acquireNextImageResult != VK_SUCCESS && m_acquireNextImageResult != VK_SUBOPTIMAL_KHR )
        {
            VX_CORE_ERROR("Vulkan Context: Failed to acquire swap chain.");
            throw std::runtime_error("Vulkan Context: Failed to acquire swap chain.");
        }
        m_SyncManager.ResetFences();
        m_CommandManager.Reset();
        
        // VX_CORE_INFO("Current frame: {0}", m_currentRenderingFrame);
        // VX_CORE_INFO("FrameBuffer Index: {0}", m_SwapChain->AvailableImageIndex);
        
        m_CommandManager.BeginRecordCommands();
        m_CommandManager.SetExtent(m_FrameBuffers[m_SwapChain->AvailableImageIndex]->Extent);
        m_CommandManager.BeginRenderPass(
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->RenderPass,
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->FrameBuffer,
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->Extent
        );
        
//        vkclass::GlobalDescriptor::Bind(m_Device.LogicalDevice, m_CommandManager);
    }

    void VulkanContext::DisplayEnd()
    {
        m_CommandManager.EndRenderPass();
        m_CommandManager.EndCommandBuffer();
        m_CommandManager.Submit(
            {m_SyncManager.GetImageAvailableSemaphore()},
            {m_SyncManager.GetRenderFinishedSemaphore()},
            m_SyncManager.GetInFlightFence()
        );
        m_presentResult = m_SwapChain->PresentImage({m_SyncManager.GetRenderFinishedSemaphore()});
        if(m_presentResult == VK_ERROR_OUT_OF_DATE_KHR || m_presentResult == VK_SUBOPTIMAL_KHR || m_framebufferResized)
        {
            //VX_CORE_TRACE("Vulkan Context: Display() present image out of date, resizing...");
            m_framebufferResized = false;
            resizeHelper();
        }else if(m_presentResult != VK_SUCCESS)
        {
            VX_CORE_ERROR("Vulkan Context: Failed to present swapchain.");
            throw std::runtime_error("Vulkan Context: Failed to present swapchain.");
        }
        m_currentRenderingFrame = (m_currentRenderingFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanContext::End()
    {
        vkDeviceWaitIdle(m_Device.LogicalDevice);
        VX_CORE_INFO("Vulkan Context: Wait Edle.");
    }

    void VulkanContext::Resize(unsigned int width, unsigned int height)
    {
        m_framebufferResized = true;
        m_Surface.SetExtent(width, height);
        VX_CORE_INFO("Vulkan Context: Resize variable set.");
    }

    void VulkanContext::createRenderPass()
    {
        vkclass::VulkanSubpass GeometrySubpass("Geometry");
        // add attachment to subpass
        vkclass::SubpassAttachmentInfo subpassInfo;
        subpassInfo.format = m_SwapChain->SurfaceFormat.format;
        GeometrySubpass.AddAttachment(subpassInfo);
        GeometrySubpass.Create();

        // create render pass (from subpass pool), with dependency
        m_RenderPass->AddSubpass(GeometrySubpass);
        m_RenderPass->AddDependency();
        m_RenderPass->Create();
    }

    void VulkanContext::resizeHelper()
    {
        //VX_CORE_TRACE("Vulkan: Resize helper delivering...");

        // if zero extent, wait, as Vulkan not allow zero extent swap chain
        if (!m_Surface.ValidateExtent())
        {
            m_framebufferResized = true;
            return;
        }

        /* recreate swap chain */
        vkDeviceWaitIdle(m_Device.LogicalDevice); // prevent touching resources that are still in used
        
        // clean up
        for(auto framebuffer: m_FrameBuffers)
        {
            delete framebuffer;
        }
        
        if(m_RenderPass != nullptr)
        {
            delete  m_RenderPass;
        }
        
        if(m_SwapChain != nullptr)
        {
            delete m_SwapChain;
        }
        
        // recreate all necessary resources
        m_SwapChain = new vkclass::VulkanSwapChain(&m_Surface);
        m_SwapChain->CreateSwapChain();
        
        // in theory, when moving a window from standard to HDR monitor, render pass should be recreated
        m_RenderPass = new vkclass::VulkanRenderPass();
        createRenderPass();
        
        createFrameBuffers();
    }

    void VulkanContext::createFrameBuffers()
    {
        // decide which frame buffers we are going to use here (e.g. deferred rendering)

        // for each swapchain image, prepare framebuffer(s) and assign renderpass
        m_FrameBuffers.resize(m_SwapChain->ImageCount);
        for(size_t i = 0; i < m_SwapChain->ImageCount; i++)
        {
            // one swapchain image : one framebuffer currently
            m_FrameBuffers[i] = new vkclass::VulkanFrameBuffer(&m_Device, m_SwapChain->Extent.width, m_SwapChain->Extent.height );
            m_FrameBuffers[i]->AddRenderPass(m_RenderPass);
            std::vector<VkImageView> imageViews{};
//            for(const auto& buffer : m_SwapChain->ImageBuffers)
//            {
                imageViews.push_back(m_SwapChain->ImageBuffers[i].view);
//            }
            m_FrameBuffers[i]->SetUpFrameBuffer(imageViews);
        }

        VX_CORE_INFO("FrameBuffers initiated.");
    }
}
