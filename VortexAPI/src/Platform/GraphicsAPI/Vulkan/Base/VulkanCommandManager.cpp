#include "VulkanCommandManager.hpp"

namespace vkclass
{
    VulkanCommandManager::VulkanCommandManager(vkclass::VulkanDevice* device):m_device(device)
    {
        
    }

    VulkanCommandManager::~VulkanCommandManager()
    {
        vkDestroyCommandPool(m_device->LogicalDevice, m_commandPool, nullptr);
        VX_CORE_INFO("VulkanCommandManager: Command Pool destroyed.");
    }

    void VulkanCommandManager::CreateCommandPools(VkCommandPoolCreateFlags flags)
    {
        VkCommandPoolCreateInfo createInfo = vkclass::initializers::commandPoolCreateInfo();
        createInfo.flags = flags;
        createInfo.queueFamilyIndex = m_device->QueueIndices.graphicsFamily.value();
        
        VK_CHECK_RESULT(vkCreateCommandPool(m_device->LogicalDevice, &createInfo, nullptr, &m_commandPool));
    }

    void VulkanCommandManager::CreateCommandBuffers()
    {
        // TODO: create one command buffer for each swap chain image and reuse command buffer
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device->LogicalDevice, &allocInfo, &m_commandBuffer));
    }

    void VulkanCommandManager::BeginRecordCommands(uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        VK_CHECK_RESULT(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
    }

    void VulkanCommandManager::BeginRenderPass(VkRenderPass renderPass, VkFramebuffer frameBuffer, VkExtent2D extent)
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

        // this should be VulkanFrameBuffers: renderPass and related framebuffer, as one framebuffer has only one renderpass
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = frameBuffer; // swapchain frame buffers[image index] (current swapchain image)

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = extent; // swapchain extent

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanCommandManager::End()
    {
        VK_CHECK_RESULT(vkEndCommandBuffer(m_commandBuffer));
    }

    void VulkanCommandManager::BindPipeline(VkPipeline pipeline)
    {
        vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }

    void VulkanCommandManager::Draw(VkExtent2D extent)
    {
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;
        vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

        vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);
    }

}
