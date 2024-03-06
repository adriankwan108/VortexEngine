#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"

namespace vkclass
{
    /**
     Encapsulate command pools, command buffers, and command operations
     */
    class VulkanCommandBuffers
    {
    public:
        explicit VulkanCommandBuffers(VkDevice device);
        ~VulkanCommandBuffers();
        
        // we need multiple command pools for multi-threading, resource separation
        // because mulitple cmd buffers create from a cmd pool, cmd pools can only be used from one thread
        // so if we want to record command buffers from multiple threads, then we will need more command pools, one per thread
        
        void CreateCommandPools(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex);
        void CreateCommandBuffers();
        void Record(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        
    private:
        VkDevice m_device;
        VkCommandPool m_commandPool; // TODO: change to vector of command pools for multi-threading and resource separation, i.e. transfer buffers, present images
        
        VkCommandBuffer m_commandBuffer;
        
    private:
    };
}
