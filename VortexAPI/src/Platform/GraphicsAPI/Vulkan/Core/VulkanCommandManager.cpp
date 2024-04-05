#include "VulkanCommandManager.hpp"

namespace vkclass
{
    VulkanCommandManager::VulkanCommandManager(vkclass::VulkanDevice* device, const int maxFramesInFlight, uint32_t& currentFrame):m_device(device), m_maxFramesInFlight(maxFramesInFlight), m_currentFrame(currentFrame)
    {
        if(device->QueueIndices.QueueFamilyIndices::isComplete())
        {
            CreateCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        }else
        {
            VX_CORE_ERROR("Vulkan: Init command buffers incomplete: queue family not complete.");
        }
        
        CreateCommandBuffers();
        VX_CORE_INFO("CommandManager initiated.");
    }

    VulkanCommandManager::~VulkanCommandManager()
    {
        vkDestroyCommandPool(m_device->LogicalDevice, m_commandPool, nullptr);
        VX_CORE_INFO("VulkanCommandManager: Command Pool destroyed.");
    }

    void VulkanCommandManager::CreateCommandPool(VkCommandPoolCreateFlags flags)
    {
        VkCommandPoolCreateInfo createInfo = vkclass::initializers::commandPoolCreateInfo();
        createInfo.flags = flags;
        createInfo.queueFamilyIndex = m_device->QueueIndices.graphicsFamily.value();
        
        VK_CHECK_RESULT(vkCreateCommandPool(m_device->LogicalDevice, &createInfo, nullptr, &m_commandPool));
    }

    void VulkanCommandManager::CreateCommandBuffers()
    {
        m_commandBuffers.resize(m_maxFramesInFlight);
        
        // Create one command buffer for each render frames (in flight frame)
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device->LogicalDevice, &allocInfo, m_commandBuffers.data()));
    }

    void VulkanCommandManager::BeginRecordCommands()
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        VK_CHECK_RESULT(vkBeginCommandBuffer(m_commandBuffers[m_currentFrame], &beginInfo));
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

        VkClearValue clearColor = { m_ClearColorValue };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_commandBuffers[m_currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanCommandManager::Reset()
    {
        // nothing special to reset flags => 0
        vkResetCommandBuffer(m_commandBuffers[m_currentFrame], 0);
    }

    void VulkanCommandManager::SetExtent(VkExtent2D extent)
    {
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_commandBuffers[m_currentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;
        vkCmdSetScissor(m_commandBuffers[m_currentFrame], 0, 1, &scissor);
    }

    void VulkanCommandManager::Submit(std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores, VkFence fence)
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        // which semaphores to wait on before execution begins
        submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        // which stage(s) of the pipeline to wait
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.pWaitDstStageMask = waitStages;
        
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffers[m_currentFrame];
        
        // which semaphores to signal once the command buffer(s) have finished execution
        submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
        submitInfo.pSignalSemaphores = signalSemaphores.data();
        
        VK_CHECK_RESULT(vkQueueSubmit(m_device->GraphicsQueue, 1, &submitInfo, fence));
    }

    void VulkanCommandManager::EndRenderPass()
    {
        vkCmdEndRenderPass(m_commandBuffers[m_currentFrame]);
    }

    void VulkanCommandManager::EndCommandBuffer()
    {
        VK_CHECK_RESULT(vkEndCommandBuffer(m_commandBuffers[m_currentFrame]));
    }

    void VulkanCommandManager::BindPipeline(VkPipeline pipeline)
    {
        vkCmdBindPipeline(m_commandBuffers[m_currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }

    void VulkanCommandManager::BindVertexBuffer(std::vector<VkBuffer> vertexBuffers, std::vector<VkDeviceSize> offsets)
    {
        vkCmdBindVertexBuffers(m_commandBuffers[m_currentFrame], 0, 1, vertexBuffers.data(), offsets.data());
    }

    void VulkanCommandManager::BindIndexBuffer(VkBuffer indexBuffer, VkDeviceSize offset)
    {
        vkCmdBindIndexBuffer(m_commandBuffers[m_currentFrame], indexBuffer, offset, VK_INDEX_TYPE_UINT16);
    }

    void VulkanCommandManager::Draw(uint32_t indexSize)
    {
//        vkCmdDraw(m_commandBuffers[m_currentFrame], 1, 1, 0, 0);
        vkCmdDrawIndexed(m_commandBuffers[m_currentFrame], indexSize, 1, 0, 0, 0);
    }

    VkCommandBuffer VulkanCommandManager::CreateCommandBuffer()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_device->LogicalDevice, &allocInfo, &commandBuffer);
        
        return commandBuffer;
    }

    void VulkanCommandManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        auto cb = CreateCommandBuffer();
        
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(cb, &beginInfo);
        
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(cb, srcBuffer, dstBuffer, 1, &copyRegion);
        
        vkEndCommandBuffer(cb);
        
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cb;

        vkQueueSubmit(m_device->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_device->GraphicsQueue);
        
        vkFreeCommandBuffers(m_device->LogicalDevice, m_commandPool, 1, &cb);
    }

    void VulkanCommandManager::SetClearColor(const glm::vec4 &color)
    {
        m_ClearColorValue.float32[0] = color.r;
        m_ClearColorValue.float32[1] = color.g;
        m_ClearColorValue.float32[2] = color.b;
        m_ClearColorValue.float32[3] = color.a;
    }

}
