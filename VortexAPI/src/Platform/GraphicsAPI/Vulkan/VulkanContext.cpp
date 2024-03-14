#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window): m_Window(window)
    {

    }

    VulkanContext::~VulkanContext()
    {
        delete triangleShader;
        
        delete m_VulkanSyncManager;
        delete m_VulkanCommandManager;

        for(auto framebuffer: m_VulkanFrameBuffers)
        {
            delete framebuffer;
        }

        delete m_RenderPass;
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
        
        vkclass::VulkanShader::Init(m_VulkanDevice->LogicalDevice);
        vkclass::VulkanPipelineBuilder::Init(m_VulkanDevice->LogicalDevice);
        
        prepareTriangle();
    }

    void VulkanContext::Display()
    {
        // all operations in display are asynchronous, inflight frames enabled
        
        // sync manager operates with current rendering frame
        // VX_CORE_INFO("Current frame: {0}", m_currentRenderingFrame);
        m_VulkanSyncManager->WaitForFences();
        m_VulkanSyncManager->ResetFences();
        
        m_VulkanSwapChain->AcquireNextImage(m_VulkanSyncManager->GetImageAvailableSemaphore()); // current frame's image available semaphore
        // VX_CORE_INFO("FrameBuffer Index: {0}", m_VulkanSwapChain->AvailableImageIndex);
        
        // cmd manager operates with current rendering frame (current rendering cmd buffer)
        m_VulkanCommandManager->Reset();
        m_VulkanCommandManager->BeginRecordCommands();
        m_VulkanCommandManager->BeginRenderPass(
            m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->RenderPass,
            m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->FrameBuffer,
            m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->Extent
        );
        
        drawTriangle();
        
        m_VulkanCommandManager->End();
        m_VulkanCommandManager->Submit(
            {m_VulkanSyncManager->GetImageAvailableSemaphore()},
            {m_VulkanSyncManager->GetRenderFinishedSemaphore()},
            m_VulkanSyncManager->GetInFlightFence()
        );
        m_VulkanSwapChain->PresentImage({m_VulkanSyncManager->GetRenderFinishedSemaphore()});
        
        m_currentRenderingFrame = (m_currentRenderingFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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
        
        // new subpass("Name")
        vkclass::VulkanSubpass GeometrySubpass("Geometry");
        // add attachment to subpass
        vkclass::SubpassAttachmentInfo subpassInfo;
        subpassInfo.format = m_VulkanSwapChain->SurfaceFormat.format;
        GeometrySubpass.AddAttachment(subpassInfo);
        GeometrySubpass.Create();
        
        // create render pass (from subpass pool), with dependency
        m_RenderPass = new vkclass::VulkanRenderPass(m_VulkanDevice->LogicalDevice);
        m_RenderPass->AddSubpass(GeometrySubpass);
        m_RenderPass->AddDependency();
        m_RenderPass->Create();
        // TODO: add to renderpass pool
        
        // for each swapchain image, prepare framebuffer(s) and assign renderpass
        m_VulkanFrameBuffers.resize(m_VulkanSwapChain->ImageCount);
        for(size_t i = 0; i < m_VulkanSwapChain->ImageCount; i++)
        {
            // one swapchain image : one framebuffer currently
            m_VulkanFrameBuffers[i] = new vkclass::VulkanFrameBuffer(m_VulkanDevice, m_VulkanSwapChain);
            m_VulkanFrameBuffers[i]->AddRenderPass(m_RenderPass);
            std::vector<VkImageView> imageViews{};
//            for(const auto& buffer : m_VulkanSwapChain->ImageBuffers)
//            {
                imageViews.push_back(m_VulkanSwapChain->ImageBuffers[i].view);
//            }
            m_VulkanFrameBuffers[i]->SetUpFrameBuffer(imageViews);
        }
        
        VX_CORE_INFO("FrameBuffers initiated.");
    }

    void VulkanContext::initCommandManager()
    {
        m_VulkanCommandManager = new vkclass::VulkanCommandManager(m_VulkanDevice, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame);
        
        if(m_VulkanDevice->QueueIndices.QueueFamilyIndices::isComplete())
        {
            m_VulkanCommandManager->CreateCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        }else
        {
            VX_CORE_ERROR("Vulkan: Init command buffers incomplete: queue family not complete.");
        }
        
        m_VulkanCommandManager->CreateCommandBuffers();
        VX_CORE_INFO("CommandManager initiated.");
    }

    void VulkanContext::initSyncManager()
    {
        m_VulkanSyncManager = new vkclass::VulkanSyncManager(m_VulkanDevice->LogicalDevice, MAX_FRAMES_IN_FLIGHT, m_currentRenderingFrame);
        VX_CORE_INFO("SyncManager initiated.");
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
        m_VulkanCommandManager->BindPipeline(triangleShader->Pipeline);
        m_VulkanCommandManager->Draw(m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->Extent);
    }
}
