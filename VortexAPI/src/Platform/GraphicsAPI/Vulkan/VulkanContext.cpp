#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window): m_Window(window)
    {

    }

    VulkanContext::~VulkanContext()
    {
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
    }

    void VulkanContext::Display()
    {
        
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
        m_VulkanFrameBuffers.resize(m_VulkanSwapChain->ImageCount);
        for(size_t i = 0; i < m_VulkanSwapChain->ImageCount; i++)
        {
            m_VulkanFrameBuffers[i] = new vkclass::VulkanFrameBuffer(m_VulkanDevice, m_VulkanSwapChain);
            
            vkclass::AttachmentCreateInfo attachmentInfo = {};
            attachmentInfo.format = m_VulkanSwapChain->SurfaceFormat.format;
//            attachmentInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT; // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
            m_VulkanFrameBuffers[i]->AddAttachment(attachmentInfo);
            m_VulkanFrameBuffers[i]->CreateRenderPass();
            m_VulkanFrameBuffers[i]->SetUpFrameBuffer();
        }
    }
}
