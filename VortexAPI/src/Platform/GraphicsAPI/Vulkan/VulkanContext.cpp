#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window): m_Window(window)
    {

    }

    VulkanContext::~VulkanContext()
    {
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
}
