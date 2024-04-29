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

    VkDescriptorSet DescriptorAllocator::Allocate(VkDescriptorSetLayout layout)
    {
        //get or create a pool to allocate from
            VkDescriptorPool poolToUse = getPool();

            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.pNext = nullptr;
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = poolToUse;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &layout;

            VkDescriptorSet targetSet;
            VkResult result = vkAllocateDescriptorSets(m_device, &allocInfo, &targetSet);

            //allocation failed. Try again
            if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL) {

                m_fullPools.push_back(poolToUse);
            
                poolToUse = getPool();
                allocInfo.descriptorPool = poolToUse;

               VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device, &allocInfo, &targetSet));
            }
          
            m_readyPools.push_back(poolToUse);
            return targetSet;
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

    //void VulkanDescriptorManager::createDescriptorSetLayout()
    //{
    //    // specify the actual buffer / image resources layout
    //    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings =
    //    {
    //        vkclass::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
    //        vkclass::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, 1)
    //    };

    //    VkDescriptorSetLayoutCreateInfo descriptorLayout = vkclass::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
    //    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device->LogicalDevice, &descriptorLayout, nullptr, &m_descriptorSetLayout));
    //}

    //void VulkanDescriptorManager::allocateDescriptorSet(VulkanBuffer& buffer)
    //{
    //    VkDescriptorSetAllocateInfo allocInfo = vkclass::initializers::descriptorSetAllocateInfo(m_uboPool, &m_descriptorSetLayout, 1);
    //    VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device->LogicalDevice, &allocInfo, &m_descriptorSet));

    //    std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
    //        // Binding 0 : Projection/View matrix as uniform buffer
    //        // vkclass::initializers::writeDescriptorSet(m_descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformBuffers.view.descriptor),
    //        // Binding 1 : Instance matrix as dynamic uniform buffer
    //        vkclass::initializers::writeDescriptorSet(m_descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, buffer.GetDescriptor())
    //    };
    //    vkUpdateDescriptorSets(m_device->LogicalDevice, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
    //}

    /*const VkDescriptorSetLayout& VulkanDescriptorManager::GetDescriptorSetLayout()
    {
        return m_descriptorSetLayout;
    }*/

    // static wrapper functions
    void DescriptorManager::Init(VulkanDescriptorManager *manager)
    {
        s_manager = manager;
    }

    /*const VkDescriptorSetLayout& DescriptorManager::GetDescriptorSetLayout()
    {
        return s_manager->GetDescriptorSetLayout();
    }*/
}
