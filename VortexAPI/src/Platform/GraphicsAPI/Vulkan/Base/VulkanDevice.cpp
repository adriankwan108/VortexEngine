#include "VulkanDevice.hpp"

namespace vkclass
{
    VulkanDevice::VulkanDevice(VkPhysicalDevice gpu)
    {
        this->m_PhysicalDevice = gpu;
        
        // store the chosen gpu's properties, features, limits
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_Properties);
        vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_SupportedFeatures);
        
        // retrieve the list of queue families
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount , nullptr);
        VX_CORE_ASSERT(queueFamilyCount > 0, "VulkanDevice: Unexpected failure: no queue family is found.");
        m_QueueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.data());
        
        // get list of supported extensions
        int i = 0;
        for (const auto& queueFamily : m_QueueFamilyProperties)
        {
            if( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_QueueFamilyIndices.graphicsFamily = i;
            }else if( (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)==0) )
            {
                // dedicated queue for compute
                m_QueueFamilyIndices.computeFamily = i;
            }else if((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)==0) && ((queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
            {
                // dedicated queue for transfer
                m_QueueFamilyIndices.transferFamily = i;
            }
            
            if(m_QueueFamilyIndices.isComplete())
            {
                break;
            }
            i++;
        }
        
        createLogicalDevice();
    }

    VulkanDevice::~VulkanDevice()
    {
        
    }
    
    bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        
        QueueFamilyIndices queueFamilyIndices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount , nullptr);
        VX_CORE_ASSERT(queueFamilyCount > 0, "VulkanDevice: Unexpected failure: no queue family is found.");
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        
        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
//            if( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//            {
//                queueFamilyIndices.graphicsFamily = i;
//            }else if( (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)==0) )
//            {
//                // dedicated queue for compute
//                queueFamilyIndices.computeFamily = i;
//            }else if((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)==0) && ((queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
//            {
//                // dedicated queue for transfer
//                queueFamilyIndices.transferFamily = i;
//            }
            
            if( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT )
            {
                queueFamilyIndices.graphicsFamily = i;
            }else if(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT )
            {
                queueFamilyIndices.computeFamily = i;
            }else if(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT )
            {
                queueFamilyIndices.transferFamily = i;
            }
                
            if(queueFamilyIndices.isComplete())
            {
                break;
            }
            i++;
        }
        VX_CORE_INFO("VulkanDevice: device type == discrete gpu: {0}", deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
        
        // macos does not support geometry shader ?
        VX_CORE_INFO("VulkanDevice: device features: geometry shader: {0}", deviceFeatures.geometryShader);
        
        VX_CORE_INFO("VulkanDevice: queue family\nGraphics: {0}\nCompute: {1}\nTransfer: {2}", queueFamilyIndices.graphicsFamily.has_value(), queueFamilyIndices.computeFamily.has_value(), queueFamilyIndices.transferFamily.has_value());

//        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader && queueFamilyIndices.isComplete();
        return queueFamilyIndices.isComplete();
    }

    void VulkanDevice::createLogicalDevice()
{
        // specify the queues to be created
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = m_QueueFamilyIndices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1; // no need more than one as we can create all of the command buffers on multiple threads and then submit them all at once on the main thread with a single low-overhead cal
        
        const float defaultQueuePriority = 0.0f;
        queueCreateInfo.pQueuePriorities = &defaultQueuePriority;
        
        VkPhysicalDeviceFeatures deviceFeatures{}; // e.g. geometry shader
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        
        createInfo.pEnabledFeatures = &deviceFeatures;
        
        createInfo.enabledExtensionCount = 0;
        if(true) // enable validation
        {
            
        }
        
    }
}
