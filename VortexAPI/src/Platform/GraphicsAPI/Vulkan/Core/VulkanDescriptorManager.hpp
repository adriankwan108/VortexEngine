#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <glm/glm.hpp>

#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"

namespace vkclass
{
    /*
     * Manager that controls the descriptor pools, and cache descriptors
     */
    class VulkanDescriptorManager
    {
    public:
        VulkanDescriptorManager(VulkanDevice* device);
        ~VulkanDescriptorManager();
        
        VkDescriptorSetLayout& GetDescriptorSetLayout();
        void allocateDescriptorSet(VulkanBuffer& buffer);
    private:
        void createDescriptorPools();
        void createDescriptorSetLayout();
        
    private:
        VulkanDevice* m_device;
        VkDescriptorPool m_uboPool;
        
        // TODO: use cache
        VkDescriptorSetLayout m_descriptorSetLayout;
        VkDescriptorSet m_descriptorSet;
    };

    
    class DescriptorManager
    {
    // static wrapper of VulkanDescriptorManager
    public:
        static void Init(VulkanDescriptorManager* manager);
        static const VkDescriptorSetLayout& GetDescriptorSetLayout();
    private:
        static VulkanDescriptorManager* s_manager;
    };
}
