#include "Buffer.hpp"
#include "Renderer.hpp"

#include "Platform/GraphicsAPI/Vulkan/Core/VulkanBuffer.hpp"

namespace VX
{
    Ref<VertexBuffer> VertexBuffer::Create()
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::None is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::Vulkan:
                return CreateRef<vkclass::VulkanVertexBuffer>();
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

    Ref<VertexBuffer> VertexBuffer::Create(void* data, uint64_t size)
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::None is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::Vulkan:
                return CreateRef<vkclass::VulkanVertexBuffer>(data, size);
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

    void IndexBuffer::AddDrawCmd(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset, int32_t offsetX, int32_t offsetY, uint32_t width, uint32_t height)
    {
        DrawCmdInfo info;
        info.indexCount = indexCount;
        info.firstIndex = firstIndex;
        info.vertexOffset = vertexOffset;
        
        info.offsetX = offsetX;
        info.offsetY = offsetY;
        info.width = width;
        info.height = height;
        
        m_drawInfoList.push_back(info);
    }

    Ref<IndexBuffer> IndexBuffer::Create()
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::None is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::Vulkan:
                return CreateRef<vkclass::VulkanIndexBuffer>();
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

    Ref<IndexBuffer> IndexBuffer::Create(void* data, uint64_t size)
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VX_CORE_ASSERT(false, "Vertex Buffer: RendererAPI::None is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::Vulkan:
                return CreateRef<vkclass::VulkanIndexBuffer>(data, size);
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

    Ref<UniformBuffer> UniformBuffer::Create(uint64_t size)
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VX_CORE_ASSERT(false, "Uniform Buffer: RendererAPI::None is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::Vulkan:
                return CreateRef<vkclass::VulkanUniformBuffer>(size);
                break;
            case RendererAPI::API::DX12:
                VX_CORE_ASSERT(false, "Uniform Buffer: RendererAPI::DX12 is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
                VX_CORE_ASSERT(false, "Uniform Buffer: RendererAPI::OpenGL is currently not supported!");
                return nullptr;
                break;
            default:
                VX_CORE_ASSERT(false, "Uniform Buffer: RendererAPI::<API> is not identified!");
                return nullptr;
                break;
        }
        return nullptr;
    }
}
