#include "VulkanDescriptorManager.hpp"

namespace vkclass
{
    VulkanDescriptorManager* DescriptorManager::s_manager = nullptr;

    VulkanDescriptorManager::VulkanDescriptorManager(VulkanDevice* device):
        m_device(device)
    {
        createDescriptorPools();
        createDescriptorSetLayout();
    }

    VulkanDescriptorManager::~VulkanDescriptorManager()
    {
        vkDestroyDescriptorPool(m_device->LogicalDevice, m_uboPool, nullptr);
        vkDestroyDescriptorSetLayout(m_device->LogicalDevice, m_descriptorSetLayout, nullptr);
    }

    void VulkanDescriptorManager::createDescriptorPools()
    {
        // currently just one pool for uniform buffers
        std::vector<VkDescriptorPoolSize> poolSizes = {
            // vkclass::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
            vkclass::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1)
        };
        VkDescriptorPoolCreateInfo descriptorPoolInfo = vkclass::initializers::descriptorPoolCreateInfo(poolSizes, 1);
        VK_CHECK_RESULT(vkCreateDescriptorPool(m_device->LogicalDevice, &descriptorPoolInfo, nullptr, &m_uboPool));
    }

    void VulkanDescriptorManager::createDescriptorSetLayout()
    {
        // specify the actual buffer / image resources layout
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings =
        {
            vkclass::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, 0)
        };

        VkDescriptorSetLayoutCreateInfo descriptorLayout = vkclass::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device->LogicalDevice, &descriptorLayout, nullptr, &m_descriptorSetLayout));
    }

    void VulkanDescriptorManager::allocateDescriptorSet(VulkanBuffer& buffer)
    {
        VkDescriptorSetAllocateInfo allocInfo = vkclass::initializers::descriptorSetAllocateInfo(m_uboPool, &m_descriptorSetLayout, 1);
        VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device->LogicalDevice, &allocInfo, &m_descriptorSet));

        std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
            // Binding 0 : Projection/View matrix as uniform buffer
            //vkclass::initializers::writeDescriptorSet(m_descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformBuffers.view.descriptor),
            // Binding 1 : Instance matrix as dynamic uniform buffer
            vkclass::initializers::writeDescriptorSet(m_descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 0, buffer.GetDescriptor())
        };
        vkUpdateDescriptorSets(m_device->LogicalDevice, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
    }

    const VkDescriptorSetLayout& VulkanDescriptorManager::GetDescriptorSetLayout()
    {
        return m_descriptorSetLayout;
    }

    // static wrapper functions
    void DescriptorManager::Init(VulkanDescriptorManager *manager)
    {
        s_manager = manager;
    }

    const VkDescriptorSetLayout& DescriptorManager::GetDescriptorSetLayout()
    {
        return s_manager->GetDescriptorSetLayout();
    }
}
