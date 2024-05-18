#include "VulkanDescriptorManager.hpp"
#include "VulkanInitializer.hpp"

namespace vkclass
{
    void DescriptorAllocator::Init(VkDevice device, int maxSets, std::vector<PoolSizeRatio> ratios, float growthRate, uint32_t maxSetLimit, bool isGrowable)
    {
        m_device = device;
        m_ratios = ratios;
        m_growthRate = growthRate;
        m_maxSetLimit = maxSetLimit;
        m_isGrowable = isGrowable;

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
            if (pool != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorPool(m_device, pool, nullptr);
            }
        }
        m_readyPools.clear();

        for (auto& pool : m_fullPools)
        {
            if (pool != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorPool(m_device, pool, nullptr);
            }
        }
        m_fullPools.clear();
        
        m_currentPtr = nullptr;
    }

    void DescriptorAllocator::Reset()
    {
        /*for (auto& p : m_readyPools) 
        {
            vkResetDescriptorPool(m_device, p, 0);
        }*/

        for (auto p : m_fullPools) 
        {
            vkResetDescriptorPool(m_device, p, 0);
            m_readyPools.push_back(p);
        }

        m_fullPools.clear();
        m_currentPtr = nullptr;
    }

    void DescriptorAllocator::Allocate(VkDescriptorSetLayout* layout, VkDescriptorSet* targetSet)
    {
        //get or create a pool to allocate from
        VkDescriptorPool poolToUse;
        /*if (m_currentPtr == nullptr)
        {
            poolToUse = grabPool();
            m_currentPtr = &poolToUse;
        }else
        {
            VX_CORE_TRACE("Descriptor Allocator: Use current pool");
            poolToUse = *m_currentPtr;
        }*/
        poolToUse = grabPool();

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.pNext = nullptr;
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = poolToUse;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layout;

        VkResult result = vkAllocateDescriptorSets(m_device, &allocInfo, targetSet);

        //allocation failed. Try again
        if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL) 
        {
            VX_CORE_TRACE("DescriptorAllocator: out of pool memory || fragmented, Try growing...");
            
            m_fullPools.push_back(poolToUse);
        
            poolToUse = grabPool();
            m_currentPtr = &poolToUse;
            allocInfo.descriptorPool = poolToUse;

           VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device, &allocInfo, targetSet));
        }
        else if(result != VK_SUCCESS)
        {
            VX_CORE_ERROR("DescriptorAllocator: Failed to allocate descriptor sets!");
            throw std::runtime_error("failed to allocate descriptor sets!");
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

    VkDescriptorPool DescriptorAllocator::grabPool()
    {
        VkDescriptorPool newPool;
        if (m_isGrowable)
        {
            if (m_readyPools.size() != 0) {
                newPool = m_readyPools.back();
                m_readyPools.pop_back();
            }
            else
            {
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
        else
        {
            newPool = m_readyPools.back();
            m_readyPools.pop_back();
        }
        return newPool;
    }


    void DescriptorWriter::WriteImage(int binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout, VkDescriptorType type)
    {
        if (imageView == VK_NULL_HANDLE)
        {
            VX_CORE_TRACE("DescriptorManager: WriteImage: null imageView");
        }
        else
        {
            VX_CORE_TRACE("DescriptorManager: WriteImage: imageView pass");
        }
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = imageView;
        imageInfo.sampler = sampler;

        VkWriteDescriptorSet write = { .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.dstBinding = binding;
        write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
        write.descriptorCount = 1;
        write.descriptorType = type;
        write.pImageInfo = &imageInfo;

        writes.push_back(write);
    }

    void DescriptorWriter::WriteImage(int binding, VkDescriptorImageInfo* imgInfo, VkDescriptorType type)
    {
        VkWriteDescriptorSet write = { .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.dstBinding = binding;
        write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
        write.descriptorCount = 1;
        write.descriptorType = type;
        write.pImageInfo = imgInfo;

        writes.push_back(write);
    }

    void DescriptorWriter::WriteBuffer(int binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type)
    {
        VkWriteDescriptorSet write = { .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };

        write.dstBinding = binding;
        write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
        write.descriptorCount = 1;
        write.descriptorType = type;
        write.pBufferInfo = bufferInfo;

        writes.push_back(write);
    }

    void DescriptorWriter::Clear()
    {
        writes.clear();
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

    void VulkanDescriptor::SetStage(VkShaderStageFlags shaderStages)
    {
        m_stage = shaderStages;
    }

    void VulkanDescriptor::AddBinding(int binding, VulkanUniformBuffer* buffer)
    {
        m_layoutBuilder.AddBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        
        for(int i = 0; i < m_maxFramesInFlight; i++)
        {
            m_writers[i].WriteBuffer(binding, buffer->GetBuffersInfo()[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        }
    }

    void VulkanDescriptor::AddBinding(int binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout)
    {
        m_layoutBuilder.AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

        for (int i = 0; i < m_maxFramesInFlight; i++)
        {
            m_writers[i].WriteImage(binding, imageView, sampler, layout, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
        }
    }

    void VulkanDescriptor::AddBinding(int binding, VkDescriptorImageInfo* imgInfo)
    {
        m_layoutBuilder.AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

        for (int i = 0; i < m_maxFramesInFlight; i++)
        {
            m_writers[i].WriteImage(binding, imgInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
        }
    }

    void VulkanDescriptor::Build()
    {
        m_layout = m_layoutBuilder.Build(m_device, m_stage);
        VX_CORE_TRACE("VulkanDescriptor: Layout built");
    }

    void VulkanDescriptor::Allocate()
    {
        DescriptorManager::Allocate(m_layout, m_setsInFlight);
        VX_CORE_TRACE("VulkanDescriptor: Allocated");
    }

    void VulkanDescriptor::Update()
    {
        for(int i = 0; i < m_maxFramesInFlight; i++)
        {
            m_writers[i].UpdateSet(m_device, m_setsInFlight[i]);
        }
        VX_CORE_TRACE("VulkanDescriptor: Updated");
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
        m_allocators[m_currentFrame].Reset();
    }

    VX::Ref<VulkanDescriptor> VulkanDescriptorManager::CreateDescriptor()
    {
        return VX::CreateRef<VulkanDescriptor>(m_device->LogicalDevice, m_maxFramesInFlight, m_currentFrame);
    }

    void VulkanDescriptorManager::Allocate(VkDescriptorSetLayout& layout, std::vector<VkDescriptorSet>& sets)
    {
        for (int i = 0; i < sets.size(); i++)
        {
            m_allocators[i].Allocate(&layout, &(sets[i]));
        }
    }


    // static wrapper functions
    void DescriptorManager::Init(VulkanDescriptorManager* manager)
    {
        s_manager = manager;
    }

    VX::Ref<VulkanDescriptor> DescriptorManager::CreateDescriptor()
    {
        return s_manager->CreateDescriptor();
    }

    void DescriptorManager::Allocate(VkDescriptorSetLayout& layout, std::vector<VkDescriptorSet>& sets)
    {
        s_manager->Allocate(layout, sets);
    }


    std::shared_ptr<VulkanDescriptor> GlobalDescriptor::s_descriptor = nullptr;

    void GlobalDescriptor::SetDescriptor(VX::Ref<VulkanDescriptor> descriptor)
    {
        s_descriptor = descriptor;
        s_descriptor->Allocate();
        s_descriptor->Update();
        VX_CORE_INFO("GlobalDescriptor: Set.");
    }

    void GlobalDescriptor::Remove()
    {
        s_descriptor.reset();
    }

    void GlobalDescriptor::Update()
    {
        // TODO:
        // if need to update descriptor set layout
        // delete current vulkandescriptor, create a new one
        // then, allocate and update
    }

    void GlobalDescriptor::Bind(VkDevice device, VulkanCommandManager* cmdManager)
    {
        // create temp pipeline layout
        VkPipelineLayoutCreateInfo layoutCreateInfo = {};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutCreateInfo.setLayoutCount = 1; // Number of descriptor set layouts to bind
        layoutCreateInfo.pSetLayouts = &s_descriptor->layout; // The descriptor set layout for the view resources

        VkPipelineLayout temporaryPipelineLayout;
        vkCreatePipelineLayout(device, &layoutCreateInfo, nullptr, &temporaryPipelineLayout);

        cmdManager->BindDescriptor(temporaryPipelineLayout, &s_descriptor->GetCurrentSet());
        vkDestroyPipelineLayout(device, temporaryPipelineLayout, nullptr);

        // VX_CORE_INFO("GlobalDescriptor: Bind");
    }
}
