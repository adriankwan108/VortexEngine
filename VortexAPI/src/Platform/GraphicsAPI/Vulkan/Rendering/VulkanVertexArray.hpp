#pragma once
#include "VortexPCH.hpp"

#include <memory>
#include <vector>
#include "vulkan/vulkan.h"

#include "Core/VulkanTools.hpp"
#include "Core/VulkanBuffer.hpp"
#include "Core/VulkanCommandManager.hpp"

#include "Renderer/VertexArray.hpp"

namespace vkclass
{
    class VulkanVertexArray : public VX::VertexArray
    {
    public:
        VulkanVertexArray();
        virtual ~VulkanVertexArray();
        
        virtual void Bind() const override;
        virtual void Unbind() const override;

    protected:
        virtual void AddApiVertexBuffer(VX::Ref<VX::VertexBuffer>& vertexBuffer) override;
        virtual void SetApiIndexBuffer(VX::Ref<VX::IndexBuffer>& indexBuffer) override;

    // vulkan specific
    public:
        static void Init(vkclass::VulkanCommandManager* commandBufferManager);

    private:
        static vkclass::VulkanCommandManager* s_commandBufferManager;
        
        std::vector<VkBuffer> m_vkVertexBuffers;
        std::vector<VkDeviceSize> m_vkOffsets;
        uint32_t m_lastStride = 0;
        
        VkBuffer m_vkIndexBuffer = VK_NULL_HANDLE;
    };
}
