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
        VulkanSyncManager(VkDevice device, const int maxFramesInFlight, uint32_t& currentFrame);
        ~VulkanSyncManager();
        
        void WaitForFences();
        void ResetFences();
        
    public:
//        const VkSemaphore& ImageSemaphore = GetImageAvailableSemaphore();
//        const VkSemaphore& RenderSemaphore = GetRenderFinishedSemaphore();
//        const VkFence& InFlightFence = GetInFlightFence();
        const VkSemaphore& GetImageAvailableSemaphore() const { return m_imageAvailableSemaphores[m_currentFrame]; }
        const VkSemaphore& GetRenderFinishedSemaphore() const { return m_renderFinishedSemaphores[m_currentFrame]; }
        const VkFence& GetInFlightFence() const { return m_inFlightFences[m_currentFrame]; }
    private:
        VkDevice m_device;
        
        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        
        int m_maxFramesInFlight = 1;
        uint32_t& m_currentFrame;
        
    };
}
