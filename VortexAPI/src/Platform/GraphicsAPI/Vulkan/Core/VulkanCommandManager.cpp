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
        
        createMainCommandBuffers();
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

    void VulkanCommandManager::CreateCommandBuffer(VkCommandBuffer* cb)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPool;
        allocInfo.commandBufferCount = 1;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device->LogicalDevice, &allocInfo, cb));
    }

    void VulkanCommandManager::CreateCommandBuffers(std::vector<VkCommandBuffer>* cbs)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPool;
        allocInfo.commandBufferCount = static_cast<uint32_t>(cbs->size());

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device->LogicalDevice, &allocInfo, cbs->data()));
    }

    void VulkanCommandManager::BeginRecordCommands()
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        VK_CHECK_RESULT(vkBeginCommandBuffer(m_mainCommandBuffers[m_currentFrame], &beginInfo));
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

        vkCmdBeginRenderPass(m_mainCommandBuffers[m_currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanCommandManager::Reset()
    {
        // nothing special to reset flags => 0
        vkResetCommandBuffer(m_mainCommandBuffers[m_currentFrame], 0);
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
        vkCmdSetViewport(m_mainCommandBuffers[m_currentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;
        vkCmdSetScissor(m_mainCommandBuffers[m_currentFrame], 0, 1, &scissor);
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
        submitInfo.pCommandBuffers = &m_mainCommandBuffers[m_currentFrame];
        
        // which semaphores to signal once the command buffer(s) have finished execution
        submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
        submitInfo.pSignalSemaphores = signalSemaphores.data();
        
        VK_CHECK_RESULT(vkQueueSubmit(m_device->GraphicsQueue, 1, &submitInfo, fence));
    }

    void VulkanCommandManager::EndRenderPass()
    {
        vkCmdEndRenderPass(m_mainCommandBuffers[m_currentFrame]);
    }

    void VulkanCommandManager::EndCommandBuffer()
    {
        VK_CHECK_RESULT(vkEndCommandBuffer(m_mainCommandBuffers[m_currentFrame]));
    }

    void VulkanCommandManager::BindPipeline(VkPipeline pipeline)
    {
        vkCmdBindPipeline(m_mainCommandBuffers[m_currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }

    void VulkanCommandManager::BindVertexBuffer(std::vector<VkBuffer> vertexBuffers, std::vector<VkDeviceSize> offsets)
    {
        vkCmdBindVertexBuffers(m_mainCommandBuffers[m_currentFrame], 0, 1, vertexBuffers.data(), offsets.data());
    }

    void VulkanCommandManager::BindIndexBuffer(VkBuffer indexBuffer, VkDeviceSize offset)
    {
        vkCmdBindIndexBuffer(m_mainCommandBuffers[m_currentFrame], indexBuffer, offset, VK_INDEX_TYPE_UINT16);
    }

    void VulkanCommandManager::BindDescriptor(VkPipelineLayout pipelineLayout, uint32_t setIndex, const VkDescriptorSet* descriptorSetPtr)
    {
        vkCmdBindDescriptorSets(m_mainCommandBuffers[m_currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, setIndex, 1, descriptorSetPtr, 0, nullptr);

    }

    void VulkanCommandManager::Draw(uint32_t indexSize)
    {
//        vkCmdDraw(m_commandBuffers[m_currentFrame], 1, 1, 0, 0);
        vkCmdDrawIndexed(m_mainCommandBuffers[m_currentFrame], indexSize, 1, 0, 0, 0);
    }

    void VulkanCommandManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        /* begin single time cmd */
        VkCommandBuffer disposbleCb;
        CreateCommandBuffer(&disposbleCb);
        
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(disposbleCb, &beginInfo);
        
        /* Copy */
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(disposbleCb, srcBuffer, dstBuffer, 1, &copyRegion);
        
        /* end single time cmd */
        vkEndCommandBuffer(disposbleCb);
        
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &disposbleCb;
        vkQueueSubmit(m_device->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_device->GraphicsQueue);
        
        vkFreeCommandBuffers(m_device->LogicalDevice, m_commandPool, 1, &disposbleCb);
    }

    void VulkanCommandManager::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        /* begin single time cmd */
        VkCommandBuffer disposbleCb;
        CreateCommandBuffer(&disposbleCb);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(disposbleCb, &beginInfo);

        /* Copy */
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            disposbleCb,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        /* end single time cmd */
        vkEndCommandBuffer(disposbleCb);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &disposbleCb;
        vkQueueSubmit(m_device->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_device->GraphicsQueue);

        vkFreeCommandBuffers(m_device->LogicalDevice, m_commandPool, 1, &disposbleCb);
    }

    void VulkanCommandManager::TransitImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        /* begin single time cmd */
        VkCommandBuffer disposbleCb;
        CreateCommandBuffer(&disposbleCb);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(disposbleCb, &beginInfo);

        /* Transit */
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;

        // don't care about the existing contents of the image
        // unless we transter queue family ownership
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        barrier.srcAccessMask = 0; // TODO
        barrier.dstAccessMask = 0; // TODO

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            disposbleCb,
            sourceStage, destinationStage, // stage operations (before, wait)
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        /* end single time cmd */
        vkEndCommandBuffer(disposbleCb);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &disposbleCb;
        vkQueueSubmit(m_device->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_device->GraphicsQueue);

        vkFreeCommandBuffers(m_device->LogicalDevice, m_commandPool, 1, &disposbleCb);


    }

    void VulkanCommandManager::SetClearColor(const glm::vec4 &color)
    {
        m_ClearColorValue.float32[0] = color.r;
        m_ClearColorValue.float32[1] = color.g;
        m_ClearColorValue.float32[2] = color.b;
        m_ClearColorValue.float32[3] = color.a;
    }

    void VulkanCommandManager::createMainCommandBuffers()
    {
        // Create one command buffer for each render frames (in flight frame)
        m_mainCommandBuffers.resize(m_maxFramesInFlight);
        CreateCommandBuffers(&m_mainCommandBuffers);
    }

}
