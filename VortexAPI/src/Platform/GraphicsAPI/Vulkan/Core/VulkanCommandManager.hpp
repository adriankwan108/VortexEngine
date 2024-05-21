#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <glm/glm.hpp>

#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"

namespace vkclass
{
    /**
    Manage command pools, command buffers, and command operations
     */
    class VulkanCommandManager
    {
    public:
        explicit VulkanCommandManager(vkclass::VulkanDevice* device, const int maxFramesInFlight, uint32_t& currentFrame);
        ~VulkanCommandManager();
        
        /*
        * TODO: need multiple command pools for multi-threading, resource separation
        * i.e. one cmd pool per thread per framesInFlight
        */
        
        /* main thread constructor */
        void CreateCommandPool(VkCommandPoolCreateFlags flags);
        void CreateCommandBuffer(VkCommandBuffer* cb);
        void CreateCommandBuffers(std::vector<VkCommandBuffer>* cbs);

        /* main thread cb operations */
        void BeginRecordCommands();
        void BeginRenderPass(VkRenderPass renderPass, VkFramebuffer frameBuffer, VkExtent2D extent);
        void Reset();
        void Submit(std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores, VkFence fence);
        void EndRenderPass();
        void EndCommandBuffer();
        void SetExtent(VkExtent2D extent);
        void BindPipeline(VkPipeline pipeline); // TODO: bind with vertex buffer, index buffer, etc; and bind with related cmd buffer
        void BindVertexBuffer(std::vector<VkBuffer> vertexBuffers, std::vector<VkDeviceSize> offsets);
        void BindIndexBuffer(VkBuffer indexBuffer, VkDeviceSize offset = 0);
        void BindDescriptor(VkPipelineLayout pipelineLayout, uint32_t setIndex, const VkDescriptorSet* descriptorSetPtr);
        void PushConstant(VkPipelineLayout pipelineLayout, VkShaderStageFlags stage, uint32_t size, void* pValue);
        void DrawIndexed(uint32_t indexCount, uint32_t firstIndex = 0, int32_t vertexOffset = 0);

        /* main thread cb operations (disposable) */
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void TransitImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        /* main render pass operation */
        void SetClearColor(const glm::vec4 &color);
        
        int GetMaxFrameInFlight() const { return m_maxFramesInFlight; }
        uint32_t GetCurrentFrame() const { return m_currentFrame; }
        
    private:
        // references
        vkclass::VulkanDevice* m_device;
        int m_maxFramesInFlight = 1;
        uint32_t& m_currentFrame;
        
        // props
        VkCommandPool m_commandPool = VK_NULL_HANDLE; // TODO: change to vector of command pools for multi-threading and resource separation, i.e. transfer buffers, present images
        
//        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_mainCommandBuffers;
        
        VkClearColorValue m_ClearColorValue = {0.0f, 0.0f, 0.0f, 1.0f};
        VkClearDepthStencilValue m_ClearDepthStencilValue = {1.0f, 0};

    private:
        void createMainCommandBuffers();
    };
}
