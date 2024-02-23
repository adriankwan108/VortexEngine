#pragma once

#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"

namespace vkclass
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;
        
        bool isComplete()
        {
            return graphicsFamily.has_value(); // && computeFamily.has_value() && transferFamily.has_value();
        }
    };

    class VulkanDevice
    {
    public:
        explicit VulkanDevice(VkPhysicalDevice gpu);
        ~VulkanDevice();
        
        static bool IsDeviceSuitable(VkPhysicalDevice device);
        
        
        operator VkDevice() const { return m_LogivcalDevice; }
    private:
        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_LogivcalDevice;
        
        /** @brief Properties of the physical device including limits that the application can check against */
        VkPhysicalDeviceProperties m_Properties;
        /** @brief Features of the physical device that an application can use to check if a feature is supported */
        VkPhysicalDeviceFeatures m_SupportedFeatures;
        /** @brief Features that have been enabled for use on the physical device */
        VkPhysicalDeviceFeatures m_EnabledFeatures;
        
        /** @brief Queue family properties of the physical device */
        std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
        QueueFamilyIndices m_QueueFamilyIndices;
        
        /** @brief Memory types and heaps of the physical device */
        VkPhysicalDeviceMemoryProperties memoryProperties;
        
    private:
        void createLogicalDevice();
    };
}
