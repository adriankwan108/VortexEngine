#include "Buffer.hpp"
#include "Platform/GraphicsAPI/Vulkan/Base/VulkanBuffer.hpp"

namespace VX
{
    /*
     * TODO: Use reference system, 
     */

    VertexBuffer* VertexBuffer::Create()
    {
        // TODO: switch api
        // new VulkanVertexBuffer(float* vertices, uint32_t size)
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create()
    {
        // TODO: switch api
        // new VulkanIndexBuffer(uint32_t* indices, uint32_t size)
        return nullptr;
    }
}
