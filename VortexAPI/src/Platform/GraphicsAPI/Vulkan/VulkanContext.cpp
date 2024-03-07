#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window): m_Window(window)
    {

    }

    VulkanContext::~VulkanContext()
    {
        delete m_VulkanSyncManager;
        delete m_VulkanCommandManager;
        for(auto framebuffer: m_VulkanFrameBuffers)
        {
            delete framebuffer;
        }
        delete m_VulkanSwapChain;
        delete m_VulkanDevice;
        delete m_VulkanSurface;
        delete m_VulkanInstance;
        VX_CORE_INFO("Vulkan: Context cleared.");
    }

    void VulkanContext::Init()
    {
        VX_CORE_INFO("Initiating Vulkan context...");
        initInstance();
        initSurface();
        initDevice();
        initSwapChain();
        initFrameBuffers();
        initCommandManager();
        initSyncManager();
    }

    void VulkanContext::Display()
    {
        // all operations in display are asynchronous
        
        m_VulkanSyncManager->WaitForFences();
        m_VulkanSyncManager->ResetFences();
        m_VulkanSwapChain->AcquireNextImage(m_VulkanSyncManager->ImageSemaphore);
        
        m_VulkanCommandManager->Reset();
        m_VulkanCommandManager->BeginRecordCommands(m_VulkanSwapChain->AvailableImageIndex);
        m_VulkanCommandManager->BeginRenderPass(
            m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->RenderPass,
            m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->FrameBuffer,
            m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->Extent
        );
        
//        // bind pipeline
//        m_VulkanCommandManager->Draw(m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->Extent);
        
        m_VulkanCommandManager->End();
        m_VulkanCommandManager->Submit(
            {m_VulkanSyncManager->ImageSemaphore},
            {m_VulkanSyncManager->RenderSemaphore},
            m_VulkanSyncManager->InFlightFence
        );
        m_VulkanSwapChain->PresentImage({m_VulkanSyncManager->RenderSemaphore});
    }

    void VulkanContext::End()
    {
        vkDeviceWaitIdle(m_VulkanDevice->LogicalDevice);
    }

    void VulkanContext::initInstance()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        appInfo.pApplicationName = "temp app"; // TODO: should be Window Data
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: follow cmake
        
        appInfo.pEngineName = "temp engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: follow cmake
        
        m_VulkanInstance = new vkclass::VulkanInstance(appInfo, m_enableValidation);
    }

    void VulkanContext::initSurface()
    {
        m_VulkanSurface = new vkclass::VulkanSurface(m_Window, m_VulkanInstance->Instance);
    }

    void VulkanContext::initDevice()
    {
        m_VulkanDevice = new vkclass::VulkanDevice
        (
            m_VulkanInstance->Instance,
            m_VulkanInstance->RequiredDeviceExtensions,
            m_VulkanSurface->Surface,
            m_enableValidation
        );
        
    }

    void VulkanContext::initSwapChain()
    {
        m_VulkanSwapChain = new vkclass::VulkanSwapChain( m_VulkanSurface ,m_VulkanDevice);
    }

    void VulkanContext::initFrameBuffers()
    {
        // decide which frame buffers we are going to use here (e.g. deferred rendering)
        
        // for each swapchain image, we need to prepare a related framebuffer(s)
        m_VulkanFrameBuffers.resize(m_VulkanSwapChain->ImageCount);
        for(size_t i = 0; i < m_VulkanSwapChain->ImageCount; i++)
        {
            // one swapchain image : one framebuffer currently
            m_VulkanFrameBuffers[i] = new vkclass::VulkanFrameBuffer(m_VulkanDevice, m_VulkanSwapChain);
            
            // fill in framebuffer necessary info
            vkclass::AttachmentCreateInfo attachmentInfo = {};
            attachmentInfo.format = m_VulkanSwapChain->SurfaceFormat.format;
            // attachmentInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            attachmentInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            m_VulkanFrameBuffers[i]->AddAttachment(attachmentInfo, m_VulkanSwapChain->ImageBuffers[i].view);
            m_VulkanFrameBuffers[i]->CreateRenderPass();
            m_VulkanFrameBuffers[i]->SetUpFrameBuffer();
        }
    }

    void VulkanContext::initCommandManager()
    {
        m_VulkanCommandManager = new vkclass::VulkanCommandManager(m_VulkanDevice);
        
        if(m_VulkanDevice->QueueIndices.QueueFamilyIndices::isComplete())
        {
            m_VulkanCommandManager->CreateCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        }else
        {
            VX_CORE_ERROR("Vulkan: Init command buffers incomplete: queue family not complete.");
        }
        
        m_VulkanCommandManager->CreateCommandBuffer();
    }

    void VulkanContext::initSyncManager()
    {
        m_VulkanSyncManager = new vkclass::VulkanSyncManager(m_VulkanDevice->LogicalDevice);
    }
}
