#include "VulkanSyncManager.hpp"

namespace vkclass
{
    VulkanSyncManager::VulkanSyncManager(VkDevice device):m_device(device)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo = vkclass::initializers::semaphoreCreateInfo();
        
        // signal fence when created to prevent blocking indefinitely due to no previous frame
        VkFenceCreateInfo fenceCreateInfo = vkclass::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
        
        
        VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_imageAvailableSemaphore));
        
        VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_renderFinishedSemaphore));
        
        VK_CHECK_RESULT(vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_inFlightFence));
        
        VX_CORE_INFO("Vulkan Sync Manager: initiated");
    }

    VulkanSyncManager::~VulkanSyncManager()
    {
        vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(m_device, m_renderFinishedSemaphore, nullptr);
        vkDestroyFence(m_device, m_inFlightFence, nullptr);
        VX_CORE_INFO("Vulkan Sync Manager: destroyed.");
    }

    void VulkanSyncManager::WaitForFences()
    {
        // wait for all fences without timeout
        vkWaitForFences(m_device, 1, &m_inFlightFence, VK_TRUE, UINT64_MAX); // but only one fence created
    }

    void VulkanSyncManager::ResetFences()
    {
        vkResetFences(m_device, 1, &m_inFlightFence);
    }
}
