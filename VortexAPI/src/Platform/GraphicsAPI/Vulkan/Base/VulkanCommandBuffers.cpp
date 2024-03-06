#include "VulkanCommandBuffers.hpp"

namespace vkclass
{
    VulkanCommandBuffers::VulkanCommandBuffers(VkDevice device):m_device(device)
    {
        
    }

    VulkanCommandBuffers::~VulkanCommandBuffers()
    {
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    }

    void VulkanCommandBuffers::CreateCommandPools(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
    {
        VkCommandPoolCreateInfo createInfo = vkclass::initializers::commandPoolCreateInfo();
        createInfo.flags = flags;
        createInfo.queueFamilyIndex = queueFamilyIndex;
        
        VK_CHECK_RESULT(vkCreateCommandPool(m_device, &createInfo, nullptr, &m_commandPool));
    }

    void VulkanCommandBuffers::CreateCommandBuffers()
    {
        // TODO: create one command buffer for each swap chain image and reuse command buffer
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device, &allocInfo, &m_commandBuffer));
    }

    void VulkanCommandBuffers::Record(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));
    }

}
