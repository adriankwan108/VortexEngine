#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"

namespace vkclass
{
    /**
    Manage command pools, command buffers, and command operations
     */
    class VulkanCommandManager
    {
    public:
        explicit VulkanCommandManager(vkclass::VulkanDevice* device);
        ~VulkanCommandManager();
        
        // we need multiple command pools for multi-threading, resource separation
        // because mulitple cmd buffers create from a cmd pool, cmd pools can only be used from one thread
        // so if we want to record command buffers from multiple threads, then we will need more command pools, one per thread
        
        void CreateCommandPool(VkCommandPoolCreateFlags flags);
        void CreateCommandBuffer();
        void BeginRecordCommands(uint32_t imageIndex);
        void BeginRenderPass(VkRenderPass renderPass, VkFramebuffer frameBuffer, VkExtent2D extent);
        void Reset();
        void Submit(std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores, VkFence fence);
        void End();

        void BindPipeline(VkPipeline pipeline); // TODO: bind with vertex buffer, index buffer, etc; and bind with related cmd buffer
        void Draw(VkExtent2D extent);
        
    private:
        vkclass::VulkanDevice* m_device;
        VkCommandPool m_commandPool = VK_NULL_HANDLE; // TODO: change to vector of command pools for multi-threading and resource separation, i.e. transfer buffers, present images
        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
        
    private:
    };
}