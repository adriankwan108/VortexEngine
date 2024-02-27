#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window)
    {
//        m_glfwExtensions = glfwGetRequiredInstanceExtensions(&m_glfwExtensionCount);
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
        initInstance(true);
        initDevice();
    }

    void VulkanContext::Display()
    {
        
    }

    void VulkanContext::initInstance(bool enableValidation)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        appInfo.pApplicationName = "temp app"; // TODO: should be Window Data
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: follow cmake
        
        appInfo.pEngineName = "temp engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: follow cmake
        
        m_VulkanInstance = new vkclass::VulkanInstance(appInfo, enableValidation);
        
    }

    void VulkanContext::initDevice()
    {
        // get number of available GPUs with Vulkan support
//        uint32_t gpuCount = 0;
//        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr));
//        
//        if(gpuCount == 0)
//        {
//            VX_CORE_ERROR("Vulkan: No GPUs with Vulkan support is found.");
//            throw std::runtime_error("Vulkan: No GPUs with Vulkan support is found.");
//            return;
//        }
//        
//        // enumerate GPUs
//        std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
//        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, physicalDevices.data()));
//        
        // select GPUs
        // Currently, just find the first suitable GPU, as I have only one GPU in hand.
        // TODO: Score all device and pick the highest one
        // TODO: Enable certain features
//        for(const auto& gpu: physicalDevices)
//        {
//            if(vkclass::VulkanDevice::IsDeviceSuitable(gpu))
//            {
//                m_gpu = gpu;
//                break;
//            }
//        }
////        m_gpu = physicalDevices[0];
//        
//        if(m_gpu == VK_NULL_HANDLE)
//        {
//            VX_CORE_ERROR("Vulkan: Failed to find a suitable GPU.");
//            throw std::runtime_error("Vulkan: Failed to find a suitable GPU.");
//        }
//        
//        m_VulkanDevice = new vkclass::VulkanDevice(m_gpu);
     
    }
}
