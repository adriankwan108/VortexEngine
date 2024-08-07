#pragma once
#include "VortexPCH.hpp"
#include <span>

#include "vulkan/vulkan.h"
#include <glm/glm.hpp>

#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanCommandManager.hpp"

namespace vkclass
{
    struct PoolSizeRatio {
        VkDescriptorType type;
        float ratio;
    };

    /* Growable descriptor pool with allocator */
    struct DescriptorAllocator {
    /* Design:
     * First grab a pool from ready pool
     * try to allocate
     * if success
     *     add pool back to ready
     * if failed
     *     put the pool to full pools, and try to get another pool / create a new pool
     */
    public:
        void Init(VkDevice device, 
            int maxSets, std::vector<PoolSizeRatio> ratios, float growthRate, uint32_t maxSetLimit, 
            bool isGrowable = false
        );
        void Destroy();
        
        void Reset();
        void Allocate(VkDescriptorSetLayout* layout, VkDescriptorSet* targetSet);

    private:
        std::vector<VkDescriptorPool> m_fullPools;
        std::vector<VkDescriptorPool> m_readyPools;
        VkDescriptorPool* m_currentPtr = nullptr;
        
        std::vector<PoolSizeRatio> m_ratios;
        float m_growthRate = 1.0f;
        uint32_t m_setsPerPool = 1000; // next increased max sets num
        uint32_t m_maxSetLimit = 4092;
        bool m_isGrowable;
        
        VkDevice m_device = VK_NULL_HANDLE;

        // TODO: use a wrapper of descriptor pool would be better for RAII consistency throughout the project
        VkDescriptorPool createPool(uint32_t setCount, std::span<PoolSizeRatio> poolRatios);
        VkDescriptorPool grabPool();

    };

    struct DescriptorWriter {
        std::vector<VkWriteDescriptorSet> writes;

        void WriteImage(int binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout, VkDescriptorType type);
        void WriteImage(int binding, VkDescriptorImageInfo* imgInfo ,VkDescriptorType type);
        void WriteBuffer(int binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type);

        void Clear();
        void UpdateSet(VkDevice device, VkDescriptorSet set);
    };

    struct DescriptorLayoutHandle {
        VkDescriptorSetLayout Layout = VK_NULL_HANDLE;
        std::vector<VkDescriptorSetLayoutBinding> Bindings;
        VkShaderStageFlags ShaderStage;
        void AddBinding(uint32_t binding, VkDescriptorType type);
        void SetShaderStage(VkShaderStageFlags shaderStage);
        void Clear();
        void Build(VkDevice device, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);
    };

    /*
     * Descriptor set and set layout by frames in flight
     */
    //class VulkanDescriptor
    //{
    //public:
    //    VulkanDescriptor(VkDevice device, const int maxFramesInFlight, uint32_t& currentFrame);
    //    ~VulkanDescriptor();
    //    
    //    void SetStage(VkShaderStageFlags shaderStages); // assume uniform buffer now
    //    
    //    // adding both layout and writer at the same time
    //    void AddBinding(int binding, VulkanUniformBuffer* buffer);
    //    void AddBinding(int binding, VkDescriptorImageInfo* imgInfo);
    //    void AddBinding(int binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout);
    //    
    //    // build props
    //    void Build();
    //    void Allocate();
    //    void Update();
    //    
    //    // get props
    //    const VkDescriptorSetLayout& layout = m_layout;
    //    std::vector<VkDescriptorSet> GetSets() const { return m_setsInFlight; }
    //    VkDescriptorSet& GetCurrentSet() { return m_setsInFlight.at(m_currentFrame); }
    //    
    //private:
    //    int m_maxFramesInFlight = 1;
    //    uint32_t& m_currentFrame;
    //    VkDevice m_device;
    //    
    //    // helper functions
    //    DescriptorLayoutBuilder m_layoutBuilder;
    //    std::vector<DescriptorWriter> m_writers;
    //    
    //    // props
    //    VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
    //    VkShaderStageFlags m_stage = VK_SHADER_STAGE_ALL;
    //    std::vector<VkDescriptorSet> m_setsInFlight;
    //};

    /*
     * Manager that controls setting and the growable descriptor pools by frames
     */
    class VulkanDescriptorManager
    {
    public:
        VulkanDescriptorManager(VulkanDevice* device, const int maxFramesInFlight, uint32_t& currentFrame);
        ~VulkanDescriptorManager();
        
        void Reset();
        // VX::Ref<VulkanDescriptor> CreateDescriptor();
        void Allocate(VkDescriptorSetLayout& layout, std::vector<VkDescriptorSet>& sets);

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
        // static VX::Ref<VulkanDescriptor> CreateDescriptor();
        static void Allocate(VkDescriptorSetLayout& layout, std::vector<VkDescriptorSet>& sets);

    private:
        static VulkanDescriptorManager* s_manager;
    };

    //class GlobalDescriptor
    //{
    //public:
    //    static VX::Ref<VulkanDescriptor> GetDescriptor() { return s_descriptor; }
    //    static void SetDescriptor(VX::Ref<VulkanDescriptor> descriptor);

    //    static void Update();
    //    static void Bind(VkDevice device, VulkanCommandManager* cmdManager);
    //    
    //    // TODO: make a deletetion queue for descriptor would be better than RAII
    //    static void Remove();
    //
    //private:
    //    static VX::Ref<VulkanDescriptor> s_descriptor;
    //};

}
