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

        vkDestroyPipeline(m_VulkanDevice->LogicalDevice, m_pipeline, nullptr);
        vkDestroyPipelineLayout(m_VulkanDevice->LogicalDevice, m_pipelineLayout, nullptr);

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
        
        m_pipelineBuilder.SetDevice(m_VulkanDevice->LogicalDevice);
        
        prepareTriangle();
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
        
        drawTriangle();
        
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

    void VulkanContext::prepareTriangle()
    {
        vkclass::VulkanShader vertShader = vkclass::VulkanShader(VX::Utils::AbsolutePath("Resources/VortexAPI/shaders/vert.spv"));
        vkclass::VulkanShader fragShader = vkclass::VulkanShader(VX::Utils::AbsolutePath("Resources/VortexAPI/shaders/frag.spv"));
        
        if (!vertShader.Valid || !fragShader.Valid)
        {
            return;
        }
        m_pipelineBuilder.SetShaders(vertShader.ShaderModule, fragShader.ShaderModule);
        
        // TODO: pipeline layout builder
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(m_VulkanDevice->LogicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
        
        m_pipeline = m_pipelineBuilder.BuildPipeline(m_pipelineLayout, m_RenderPass->RenderPass);

    }

    void VulkanContext::drawTriangle()
    {
        m_VulkanCommandManager->BindPipeline(m_pipeline);
        m_VulkanCommandManager->Draw(m_VulkanFrameBuffers[m_VulkanSwapChain->AvailableImageIndex]->Extent);
    }
}
