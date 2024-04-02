#include "Buffer.hpp"
#include "Renderer.hpp"

#include "Platform/GraphicsAPI/Vulkan/Core/VulkanBuffer.hpp"

namespace VX
{
    /*
     * TODO: Use reference system, 
     */

    VertexBuffer* VertexBuffer::Create(void* data, uint64_t size)
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::None is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::Vulkan:
                return new vkclass::VulkanVertexBuffer(data, size);
                break;
            case RendererAPI::API::DX12:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::DX12 is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::OpenGL is currently not supported!");
                return nullptr;
                break;
            default:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::<API> is not identified!");
                return nullptr;
                break;
        }
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(void* data, uint64_t size)
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::None is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::Vulkan:
                return new vkclass::VulkanIndexBuffer(data, size);
                return nullptr;
                break;
            case RendererAPI::API::DX12:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::DX12 is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::OpenGL is currently not supported!");
                return nullptr;
                break;
            default:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::<API> is not identified!");
                return nullptr;
                break;
        }
        return nullptr;
    }
}
