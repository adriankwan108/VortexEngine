#pragma once

#include "VortexPCH.hpp"
#include <map>
#include <set>
#include "vulkan/vulkan.h"
#include "VulkanTools.hpp"

namespace vkclass
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
//        std::optional<uint32_t> computeFamily;
//        std::optional<uint32_t> transferFamily;
        
        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
        
        void ShowInfo()
        {
            VX_CORE_INFO("GraphicsFamily: {0}", graphicsFamily.has_value()
                         ?std::to_string(graphicsFamily.value())
                         :"Dedicated queue not found");
            VX_CORE_INFO("PresentFamily: {0}", presentFamily.has_value()
                         ?std::to_string(presentFamily.value())
                         :"Dedicated queue not found");
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanDevice
    {
    public:
//        explicit VulkanDevice(VkPhysicalDevice gpu);
        explicit VulkanDevice(VkInstance instance, std::vector<const char *> requiredDeviceExtensions, VkSurfaceKHR surface, bool enableValidation);
        ~VulkanDevice();
        
        const VkPhysicalDevice& PhysicalDevice = m_physicalDevice;
        const VkDevice& LogicalDevice = m_logicalDevice;
        QueueFamilyIndices& QueueIndices = m_QueueFamilyIndices;
        
        const VkQueue& GraphicsQueue = m_graphicsQueue;
        const VkQueue& PresentQueue = m_presentQueue;
        
    public:
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

    private:
        bool m_enableValidation = true;
        const std::vector<const char*> m_validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        
        // ref
        VkSurfaceKHR m_surface;
        
        // properties
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_logicalDevice;
        
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        
        VkPhysicalDeviceProperties m_gpuProps;
        VkPhysicalDeviceFeatures m_gpuFeatures;
        
        uint32_t m_gpuCount = 0;
        std::vector<VkPhysicalDevice> m_physicalDevices;
        
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
        
        /** @brief List of extensions supported by the device */
        std::vector<std::string> m_supportedDeviceExtensions;
        
        
    private:
        void getPhysicalDevice(VkInstance instance);
        void retrieveGpuInfo(VkPhysicalDevice device, VkPhysicalDeviceProperties& props, VkPhysicalDeviceFeatures& features);
        bool isGpuSuitable(VkPhysicalDevice device);
        int rateGpuSuitability(VkPhysicalDevice device);
        
        void createLogicalDevice(std::vector<const char *> requiredDeviceExtensions);
        bool isGpuExtensionSupported(std::string extension);
        
    };
}