#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window) :
        m_Window(window),
        m_Instance(m_enableValidation),
        m_Surface(window, &m_Instance),
        m_Device(&m_Instance, &m_Surface, m_enableValidation),
        m_SwapChain(&m_Device, &m_Surface),
        m_SyncManager(&m_Device, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame),
        m_CommandManager(&m_Device, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame),
        m_RenderTargetManager(m_Device.LogicalDevice, &m_SwapChain)
        // m_DescriptorManager(&m_Device, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame),
        // m_RenderPassManager()
    {
        m_SwapChain.CreateSwapChain(m_isVSync, m_isFullScreen);
        m_RenderTargetManager.CreateSwapChainTarget();
    }

    VulkanContext::~VulkanContext()
    {
        /*for(auto framebuffer: m_FrameBuffers)
        {
            delete framebuffer;
        }*/
        
        
        // vkclass::GlobalDescriptor::Remove();

        VX_CORE_INFO("Vulkan: Resources cleared.");
        VX_CORE_INFO("Vulkan: Destroying context...");
    }

    void VulkanContext::Init()
    {
        VX_CORE_INFO("VulkanContext:: Initiating RendererAPI...");
        vkclass::VulkanRendererAPI::SetCommandManager(&m_CommandManager);
        
        VX_CORE_INFO("VulkanContext:: Initiating Vulkan resources classes...");
        vkclass::VulkanRenderTarget::Init(&m_RenderTargetManager);
        
//        vkclass::VulkanRenderPass::Init(m_Device.LogicalDevice);
//        vkclass::VulkanPipelineBuilder::Init(m_Device.LogicalDevice);
//        vkclass::VulkanBuffer::Init(&m_Device, &m_CommandManager);
//        vkclass::VulkanVertexArray::Init(&m_CommandManager);
//        vkclass::VulkanCamera::Init(&m_Device);
//        vkclass::VulkanTexture2D::Init(&m_Device, &m_CommandManager);
//        vkclass::VulkanShader::Init(m_Device.LogicalDevice);
//        vkclass::VulkanShaderPass::Init(m_Device.LogicalDevice);
//        vkclass::DescriptorManager::Init(&m_DescriptorManager);
        

        // m_RenderPassManager.Init(m_SwapChain->SurfaceFormat.format);
        // vkclass::VulkanShaderEffect::Init(m_Device.LogicalDevice, &m_RenderPassManager, &m_CommandManager);
        
        
        // createFrameBuffers();
    }

    void VulkanContext::DisplayStart()
    {
        // all operations in display are asynchronous, inflight frames enabled, managers work with current rendering frame
        
        // wait for prev frame to finish
        m_SyncManager.WaitForFences();
        m_SyncManager.ResetFences();
        
        // acquire an image from swap chain
         m_acquireNextImageResult = m_SwapChain.AcquireNextImage(m_SyncManager.GetImageAvailableSemaphore());
        //if( m_acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) // btw the return state is not guranteed by drivers / platforms
        //{
        //    VX_CORE_INFO("Vulkan Context: Display() acquire image out of date, resizing...");
        //    resizeHelper();
        //    return;
        //}else if(m_acquireNextImageResult != VK_SUCCESS && m_acquireNextImageResult != VK_SUBOPTIMAL_KHR )
        //{
        //    VX_CORE_ERROR("Vulkan Context: Failed to acquire swap chain.");
        //    throw std::runtime_error("Vulkan Context: Failed to acquire swap chain.");
        //}
        
        // m_DescriptorManager.Reset();
        // VX_CORE_INFO("Current frame: {0}", m_currentRenderingFrame);
        // VX_CORE_INFO("FrameBuffer Index: {0}", m_SwapChain->AvailableImageIndex);
         
        // record command buffer
        m_CommandManager.Reset();
        m_CommandManager.BeginRecordCommands();
        
         
        //m_CommandManager.SetExtent(m_FrameBuffers[m_SwapChain->AvailableImageIndex]->Extent);
        // m_CommandManager.SetExtent(m_Surface.GetExtent());
        /*m_CommandManager.BeginRenderPass(
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->RenderPass,
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->FrameBuffer,
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->Extent
        );*/
        
         // vkclass::GlobalDescriptor::Bind(m_Device.LogicalDevice, &m_CommandManager);
    }

    void VulkanContext::DisplayEnd()
    {
        // m_CommandManager.EndRenderPass();
        
        // submit recorded command buffer
        m_CommandManager.EndCommandBuffer();
        m_CommandManager.Submit(
            {m_SyncManager.GetImageAvailableSemaphore()},
            {m_SyncManager.GetRenderFinishedSemaphore()},
            m_SyncManager.GetInFlightFence()
        );
        
        // verify framebuffer image format
        m_CommandManager.TransitImageLayout(m_SwapChain.GetCurrentImage(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        
        // present the swap chain image
         m_presentResult = m_SwapChain.PresentImage({m_SyncManager.GetRenderFinishedSemaphore()});
        //if(m_presentResult == VK_ERROR_OUT_OF_DATE_KHR || m_presentResult == VK_SUBOPTIMAL_KHR || m_framebufferResized)
        //{
        //    //VX_CORE_TRACE("Vulkan Context: Display() present image out of date, resizing...");
        //    m_framebufferResized = false;
        //    resizeHelper();
        //}else if(m_presentResult != VK_SUCCESS)
        //{
        //    VX_CORE_ERROR("Vulkan Context: Failed to present swapchain.");
        //    throw std::runtime_error("Vulkan Context: Failed to present swapchain.");
        //}
        m_currentRenderingFrame = (m_currentRenderingFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanContext::End()
    {
        vkDeviceWaitIdle(m_Device.LogicalDevice);
        VX_CORE_INFO("Vulkan Context: Wait Edle.");
    }

    void VulkanContext::Resize(unsigned int width, unsigned int height)
    {
        /*m_framebufferResized = true;
        m_Surface.SetExtent(width, height);
        VX_CORE_INFO("Vulkan Context: Resize variable set.");*/
    }

    void VulkanContext::SetVSync(bool enable)
    {
        if (m_isVSync != enable)
        {
            m_isVSync = enable;
            resizeHelper();
        }
    }

    void VulkanContext::resizeHelper()
    {
        //VX_CORE_TRACE("Vulkan: Resize helper delivering...");

        // if zero extent, wait, as Vulkan not allow zero extent swap chain
        /*if (!m_Surface.ValidateExtent())
        {
            m_framebufferResized = true;
            return;
        }*/


        // prevent touching resources that are still in used
        // vkDeviceWaitIdle(m_Device.LogicalDevice);
        
        // clean up
        /*for(auto framebuffer: m_FrameBuffers)
        {
            delete framebuffer;
        }*/
        
        /* recreate swap chain */
        // m_SwapChain->CreateSwapChain(m_isVSync, m_isFullScreen);
        
        // in theory, when moving a window from standard to HDR monitor, render pass should be recreated
        // m_RenderPassManager.Reload("main", m_SwapChain->SurfaceFormat.format);
        
        // createFrameBuffers();
    }

//    void VulkanContext::createFrameBuffers()
//    {
//        // decide which frame buffers we are going to use here (e.g. deferred rendering)
//
//        // for each swapchain image, prepare framebuffer(s) and assign renderpass
//        m_FrameBuffers.resize(m_SwapChain->ImageCount);
//        for(size_t i = 0; i < m_SwapChain->ImageCount; i++)
//        {
//            // one swapchain image : one framebuffer currently
//            m_FrameBuffers[i] = new vkclass::VulkanFrameBuffer(&m_Device, m_SwapChain->Extent.width, m_SwapChain->Extent.height );
//            m_FrameBuffers[i]->SetRenderPass(m_RenderPassManager.GetRenderPass("main"));
//            std::vector<VkImageView> imageViews{};
////            for(const auto& buffer : m_SwapChain->ImageBuffers)
////            {
//                imageViews.push_back(m_SwapChain->ImageBuffers[i].view);
////            }
//            m_FrameBuffers[i]->SetUpFrameBuffer(imageViews);
//        }
//
//        VX_CORE_INFO("FrameBuffers initiated.");
//    }

}
