#pragma once
#include "VortexPCH.hpp"
#include <span>
#include <deque>

#include "vulkan/vulkan.h"
#include <glm/glm.hpp>

#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"

namespace vkclass
{
    struct PoolSizeRatio {
        VkDescriptorType type;
        float ratio;
    };

    /* Growable descriptor pool
     * Design:
     *      First grab a pool from ready pool
     *      try to allocate
     *      if success
     *          add pool back to ready
     *      if failed
     *          put the pool to full pools, and try to get another pool / create a new pool
    */
    struct DescriptorAllocator {
    public:
        void Init(VkDevice device, int maxSets, std::vector<PoolSizeRatio> ratios, float growthRate, uint32_t maxSetLimit);
        void Destroy();
        
        void Clear();
        VkDescriptorSet Allocate(VkDescriptorSetLayout layout);
    private:
        std::vector<VkDescriptorPool> m_fullPools;
        std::vector<VkDescriptorPool> m_readyPools;
        std::vector<PoolSizeRatio> m_ratios;
        float m_growthRate = 1.0f;
        uint32_t m_setsPerPool = 1000; // next increased max sets num
        uint32_t m_maxSetLimit = 4092;
        
        VkDevice m_device = VK_NULL_HANDLE;

        VkDescriptorPool createPool(uint32_t setCount, std::span<PoolSizeRatio> poolRatios);
        VkDescriptorPool getPool();
    };

    struct DescriptorWriter {
        std::deque<VkDescriptorImageInfo> imageInfos;
        std::deque<VkDescriptorBufferInfo> bufferInfos;
        std::vector<VkWriteDescriptorSet> writes;

        void write_image(int binding, VkImageView image, VkSampler sampler, VkImageLayout layout, VkDescriptorType type);
        void write_buffer(int binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type);

        void clear();
        void update_set(VkDevice device, VkDescriptorSet set);
    };

    /*
     * Manager that controls setting and the growable descriptor pools by frames
     */
    class VulkanDescriptorManager
    {
    public:
        VulkanDescriptorManager(VulkanDevice* device, const int maxFramesInFlight, uint32_t& currentFrame);
        ~VulkanDescriptorManager();
        
        void Reset();
        
    private:
        VulkanDevice* m_device;
        int m_maxFramesInFlight = 1;
        uint32_t& m_currentFrame;

        int m_maxSets = 1000; // max sets of a pool
        float m_growthRate = 1.5;
        uint32_t m_maxSetLimit = 4092;

        std::vector<PoolSizeRatio> m_ratios = {
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4 },
        };

        std::vector<DescriptorAllocator> m_allocators; // by frame
    };

    // static descriptor manager interface
    class DescriptorManager
    {
    public:
        static void Init(VulkanDescriptorManager* manager);
        // static const VkDescriptorSetLayout& GetDescriptorSetLayout();
    private:
        static VulkanDescriptorManager* s_manager;
    };
}
