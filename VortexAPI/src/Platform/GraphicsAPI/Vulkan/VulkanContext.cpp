#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window):
        m_Window(window),
        m_Instance(m_enableValidation),
        m_Surface(window, &m_Instance),
        m_Device(&m_Instance, &m_Surface, m_enableValidation),
        m_SyncManager(&m_Device, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame),
        m_CommandManager(&m_Device, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame)
    {

    }

    VulkanContext::~VulkanContext()
    {
        delete triangleShader;
        
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

        VX_CORE_INFO("Vulkan: Resources cleared.");
        VX_CORE_INFO("Vulkan: Destroying context...");
    }

    void VulkanContext::Init()
    {
        VX_CORE_INFO("Initiating Vulkan resources...");
        
        vkclass::VulkanSwapChain::Init(&m_Device);
        vkclass::VulkanRenderPass::Init(m_Device.LogicalDevice);
        vkclass::VulkanShader::Init(m_Device.LogicalDevice);
        vkclass::VulkanPipelineBuilder::Init(m_Device.LogicalDevice);
        
        m_SwapChain = new vkclass::VulkanSwapChain(&m_Surface);
        m_SwapChain->CreateSwapChain();
        
        m_RenderPass = new vkclass::VulkanRenderPass();
        createRenderPass();
        
        createFrameBuffers();

        prepareTriangle();
    }

    void VulkanContext::Display()
    {
        // all operations in display are asynchronous, inflight frames enabled
        
        // sync manager operates objects with current rendering frame (i.e. fence, semaphores)
        // VX_CORE_INFO("Current frame: {0}", m_currentRenderingFrame);
        m_SyncManager.WaitForFences();
        
        m_acquireNextImageResult = m_SwapChain->AcquireNextImage(m_SyncManager.GetImageAvailableSemaphore());
        if( m_acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) // btw the return state is not guranteed by drivers / platforms
        {
            resizeHelper();
            return;
        }else if(m_acquireNextImageResult != VK_SUCCESS && m_acquireNextImageResult != VK_SUBOPTIMAL_KHR )
        {
            VX_CORE_ERROR("Vulkan Context: Failed to acquire swap chain.");
            throw std::runtime_error("Vulkan Context: Failed to acquire swap chain.");
        }
        m_SyncManager.ResetFences();
        // VX_CORE_INFO("FrameBuffer Index: {0}", m_SwapChain->AvailableImageIndex);
        
        // cmd manager operates objects with current rendering frame (e.g. current rendering cmd buffer)
        m_CommandManager.Reset();
        m_CommandManager.BeginRecordCommands();
        m_CommandManager.BeginRenderPass(
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->RenderPass,
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->FrameBuffer,
            m_FrameBuffers[m_SwapChain->AvailableImageIndex]->Extent
        );

        drawTriangle();
        
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
        VX_CORE_INFO("Vulkan: Resize helper delivering...");
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

    void VulkanContext::prepareTriangle()
    {
        triangleShader = new vkclass::VulkanShader(
            VX::Utils::AbsolutePath("Resources/VortexAPI/shaders/vert.spv"),
            VX::Utils::AbsolutePath("Resources/VortexAPI/shaders/frag.spv")
        );

        if (!triangleShader->Valid)
        {
            return;
        }

        triangleShader->CreatePipeline(m_RenderPass->RenderPass);
    }

    void VulkanContext::drawTriangle()
    {
        if(!triangleShader->Valid)
        {
            return;
        }
        m_CommandManager.BindPipeline(triangleShader->Pipeline);
        m_CommandManager.Draw(m_FrameBuffers[m_SwapChain->AvailableImageIndex]->Extent);
    }
}
