#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"

namespace vkclass
{
    class VulkanSyncManager
    {
    public:
        VulkanSyncManager(VkDevice device);
        ~VulkanSyncManager();
        
        void WaitForFences();
        void ResetFences();
        
    public:
        const VkSemaphore& ImageSemaphore = m_imageAvailableSemaphore;
        const VkSemaphore& RenderSemaphore = m_renderFinishedSemaphore;
        const VkFence& InFlightFence = m_inFlightFence;
    private:
        VkDevice m_device;
        
        VkSemaphore m_imageAvailableSemaphore;
        VkSemaphore m_renderFinishedSemaphore;
        VkFence m_inFlightFence;
    };
}
