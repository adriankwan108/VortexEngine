#include "VulkanSyncManager.hpp"

namespace vkclass
{
    VulkanSyncManager::VulkanSyncManager(VkDevice device, const int maxFramesInFlight, uint32_t& currentFrame):m_device(device), m_maxFramesInFlight(maxFramesInFlight), m_currentFrame(currentFrame)
    {
        m_imageAvailableSemaphores.resize(m_maxFramesInFlight);
        m_renderFinishedSemaphores.resize(m_maxFramesInFlight);
        m_inFlightFences.resize(m_maxFramesInFlight);
        
        VkSemaphoreCreateInfo semaphoreCreateInfo = vkclass::initializers::semaphoreCreateInfo();
        
        // signal fence when created to prevent blocking indefinitely due to no previous frame
        VkFenceCreateInfo fenceCreateInfo = vkclass::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
        
        for(size_t i = 0; i < m_maxFramesInFlight; i++)
        {
            VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_imageAvailableSemaphores[i]));
            
            VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_renderFinishedSemaphores[i]));
            
            VK_CHECK_RESULT(vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_inFlightFences[i]));
        }
        
        VX_CORE_INFO("Vulkan Sync Manager created");
    }

    VulkanSyncManager::~VulkanSyncManager()
    {
        for(size_t i = 0; i < m_maxFramesInFlight; i++)
        {
            vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
        }
        VX_CORE_INFO("Vulkan Sync Manager: destroyed.");
    }

    void VulkanSyncManager::WaitForFences()
    {
        // wait for all fences without timeout
        vkWaitForFences(m_device, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX); // but only one fence created
    }

    void VulkanSyncManager::ResetFences()
    {
        vkResetFences(m_device, 1, &m_inFlightFences[m_currentFrame]);
    }
}
