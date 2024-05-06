#include "VulkanDescriptorManager.hpp"

namespace vkclass
{
    void DescriptorAllocator::Init(VkDevice device, int maxSets, std::vector<PoolSizeRatio> ratios, float growthRate, uint32_t maxSetLimit)
    {
        m_device = device;
        m_ratios = ratios;
        m_growthRate = growthRate;
        m_maxSetLimit = maxSetLimit;

        VkDescriptorPool newPool = createPool(maxSets, m_ratios);
        m_setsPerPool = maxSets * m_growthRate;
        if (m_setsPerPool > m_maxSetLimit)
        {
            m_setsPerPool = m_maxSetLimit;
        }
        m_readyPools.push_back(newPool);
    }

    void DescriptorAllocator::Destroy()
    {
        for (auto& pool : m_readyPools)
        {
            vkDestroyDescriptorPool(m_device, pool, nullptr);
        }
        m_readyPools.clear();

        for (auto& pool : m_fullPools)
        {
            vkDestroyDescriptorPool(m_device, pool, nullptr);
        }
        m_fullPools.clear();
    }

    void DescriptorAllocator::Clear()
    {
        for (auto p : m_readyPools) {
                vkResetDescriptorPool(m_device, p, 0);
            }
            for (auto p : m_fullPools) {
                vkResetDescriptorPool(m_device, p, 0);
                m_readyPools.push_back(p);
            }
            m_fullPools.clear();
    }

    void DescriptorAllocator::Allocate(VkDescriptorSetLayout layout, VkDescriptorSet* targetSet)
    {
        //get or create a pool to allocate from
            VkDescriptorPool poolToUse = getPool();

            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.pNext = nullptr;
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = poolToUse;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &layout;

            VkResult result = vkAllocateDescriptorSets(m_device, &allocInfo, targetSet);

            //allocation failed. Try again
            if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL) {

                m_fullPools.push_back(poolToUse);
            
                poolToUse = getPool();
                allocInfo.descriptorPool = poolToUse;

               VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device, &allocInfo, targetSet));
            }
          
            m_readyPools.push_back(poolToUse);
    }

    VkDescriptorPool DescriptorAllocator::createPool(uint32_t setCount, std::span<PoolSizeRatio> poolRatios)
    {
        // currently just one pool for uniform buffers
        std::vector<VkDescriptorPoolSize> poolSizes;

        for (PoolSizeRatio ratio : poolRatios)
        {
            poolSizes.push_back(vkclass::initializers::descriptorPoolSize(ratio.type, static_cast<uint32_t>(ratio.ratio * setCount)));
        }
        
        VkDescriptorPool newPool;
        VkDescriptorPoolCreateInfo descriptorPoolInfo = vkclass::initializers::descriptorPoolCreateInfo(poolSizes, setCount);
        VK_CHECK_RESULT(vkCreateDescriptorPool(m_device, &descriptorPoolInfo, nullptr, &newPool));
        return newPool;
    }

    VkDescriptorPool DescriptorAllocator::getPool()
    {
        VkDescriptorPool newPool;
        if (m_readyPools.size() != 0) {
            newPool = m_readyPools.back();
            m_readyPools.pop_back();
        }
        else {
            // create a new pool
            newPool = createPool(m_setsPerPool, m_ratios);
            m_setsPerPool = m_setsPerPool * m_growthRate;
            if (m_setsPerPool > m_maxSetLimit)
            {
                m_setsPerPool = m_maxSetLimit;
            }
        }
        return newPool;
    }


    void DescriptorWriter::WriteImage(int binding, VkImageView image, VkSampler sampler, VkImageLayout layout, VkDescriptorType type)
    {
        VkDescriptorImageInfo& info = imageInfos.emplace_back(
            VkDescriptorImageInfo
            {
                .sampler = sampler,
                .imageView = image,
                .imageLayout = layout
            }
        );

        VkWriteDescriptorSet write = { .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };

        write.dstBinding = binding;
        write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
        write.descriptorCount = 1;
        write.descriptorType = type;
        write.pImageInfo = &info;

        writes.push_back(write);
    }

    void DescriptorWriter::WriteBuffer(int binding, VkDescriptorBufferInfo bufferInfo, VkDescriptorType type)
    {
        VkDescriptorBufferInfo& info = bufferInfos.emplace_back(bufferInfo);

        VkWriteDescriptorSet write = { .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };

        write.dstBinding = binding;
        write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
        write.descriptorCount = 1;
        write.descriptorType = type;
        write.pBufferInfo = &info;

        writes.push_back(write);
    }

    void DescriptorWriter::Clear()
    {
        imageInfos.clear();
        writes.clear();
        bufferInfos.clear();
    }

    void DescriptorWriter::UpdateSet(VkDevice device, VkDescriptorSet set)
    {
        for (VkWriteDescriptorSet& write : writes) 
        {
            write.dstSet = set;
        }

        vkUpdateDescriptorSets(device, (uint32_t)writes.size(), writes.data(), 0, nullptr);
    }


    void DescriptorLayoutBuilder::AddBinding(uint32_t binding, VkDescriptorType type)
    {
        VkDescriptorSetLayoutBinding newbind{};
        newbind.binding = binding;
        newbind.descriptorCount = 1;
        newbind.descriptorType = type;

        bindings.push_back(newbind);
    }

    void DescriptorLayoutBuilder::Clear()
    {
        bindings.clear();
    }

    VkDescriptorSetLayout DescriptorLayoutBuilder::Build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext, VkDescriptorSetLayoutCreateFlags flags)
    {
        for (auto& b : bindings) 
        {
            b.stageFlags |= shaderStages;
        }

        VkDescriptorSetLayoutCreateInfo info = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
        info.pNext = pNext;

        info.pBindings = bindings.data();
        info.bindingCount = (uint32_t)bindings.size();
        info.flags = flags;

        VkDescriptorSetLayout set;
        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &info, nullptr, &set));

        return set;
    }

    VulkanDescriptor::VulkanDescriptor(VkDevice device, const int maxFramesInFlight, uint32_t& currentFrame): m_device(device), m_maxFramesInFlight(maxFramesInFlight), m_currentFrame(currentFrame)
    {
        m_setsInFlight.resize(m_maxFramesInFlight);
        m_writers.resize(m_maxFramesInFlight);
    }

    VulkanDescriptor::~VulkanDescriptor()
    {
        vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
        m_setsInFlight.clear();
    }

    void VulkanDescriptor::AddBinding(int binding, VulkanUniformBuffer* buffer)
    {
        m_buffer = buffer;
        
        m_layoutBuilder.AddBinding(binding, m_type);
        
        for(int i = 0; i < m_maxFramesInFlight; i++)
        {
            m_writers[i].WriteBuffer(binding, buffer->GetBuffersInfo()[i], m_type);
        }
    }

    void VulkanDescriptor::AddBinding(int binding, VkImageView image, VkSampler sampler, VkImageLayout layout)
    {
        m_layoutBuilder.AddBinding(binding, m_type);
        // m_writer.WriteImage(binding, image, sampler, layout, m_type);
    }

    void VulkanDescriptor::Build()
    {
        VX_CORE_INFO("Building Descriptor...");
        m_layout = m_layoutBuilder.Build(m_device, VK_SHADER_STAGE_VERTEX_BIT);
        m_setsInFlight = DescriptorManager::Allocate(m_layout);

        for(int i = 0; i < m_maxFramesInFlight; i++)
        {
            m_writers[i].UpdateSet(m_device, m_setsInFlight[i]);
        }
        
        updated = true;
    }

    VulkanDescriptorManager* DescriptorManager::s_manager = nullptr;

    VulkanDescriptorManager::VulkanDescriptorManager(VulkanDevice* device, const int maxFramesInFlight, uint32_t& currentFrame):
        m_device(device), m_maxFramesInFlight(maxFramesInFlight), m_currentFrame(currentFrame)
    {
        m_allocators.resize(m_maxFramesInFlight);

        for (auto& allocator : m_allocators)
        {
            allocator.Init(m_device->LogicalDevice, m_maxSets, m_ratios, m_growthRate, m_maxSetLimit);
        }
    }

    VulkanDescriptorManager::~VulkanDescriptorManager()
    {
        for (auto& allocator : m_allocators)
        {
            allocator.Destroy();
        }
    }

    void VulkanDescriptorManager::Reset()
    {
        m_allocators[m_currentFrame].Clear();
    }

    std::shared_ptr<VulkanDescriptor> VulkanDescriptorManager::CreateDescriptor()
    {
        return std::shared_ptr<VulkanDescriptor>(new VulkanDescriptor(m_device->LogicalDevice, m_maxFramesInFlight, m_currentFrame));
    }

    std::vector<VkDescriptorSet> VulkanDescriptorManager::Allocate(VkDescriptorSetLayout layout)
    {
        std::vector<VkDescriptorSet> sets;
        sets.resize(m_maxFramesInFlight);
        
        for (int i = 0; i < m_maxFramesInFlight; i++)
        {
            m_allocators[i].Allocate(layout, &sets[i]);
        }

        return sets;
    }

    // static wrapper functions
    void DescriptorManager::Init(VulkanDescriptorManager *manager)
    {
        s_manager = manager;
    }

    std::shared_ptr<VulkanDescriptor> DescriptorManager::CreateDescriptor()
    {
        return s_manager->CreateDescriptor();
    }

    std::vector<VkDescriptorSet> DescriptorManager::Allocate(VkDescriptorSetLayout layout)
    {
        return s_manager->Allocate(layout);
    }

    std::shared_ptr<VulkanDescriptor> GlobalDescriptor::s_descriptor = nullptr;

    void GlobalDescriptor::SetDescriptor(std::shared_ptr<VulkanDescriptor> descriptor)
    {
        if(!descriptor->updated)
        {
            VX_CORE_WARN("GlobalDescriptor: descriptor not yet updated.");
        }
        s_descriptor = descriptor;
    }

    void GlobalDescriptor::Remove()
    {
        s_descriptor.reset();
    }
}
