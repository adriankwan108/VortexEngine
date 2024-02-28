#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window): m_Window(window)
    {

    }

    VulkanContext::~VulkanContext()
    {
        delete m_VulkanDevice;
        delete m_VulkanInstance;
        VX_CORE_INFO("Vulkan: Context cleared.");
    }

    void VulkanContext::Init()
    {
        VX_CORE_INFO("Initiating Vulkan context...");
        initInstance();
        initDevice();
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
        
        m_VulkanInstance->CreateSurface(m_Window);
    }

    void VulkanContext::initDevice()
    {
        m_VulkanDevice = new vkclass::VulkanDevice
        (
            m_VulkanInstance->Instance,
            m_VulkanInstance->RequiredDeviceExtensions,
            m_VulkanInstance->Surface,
            m_enableValidation
        );
        
    }
}
