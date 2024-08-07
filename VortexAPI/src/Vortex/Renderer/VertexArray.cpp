#include "VertexArray.hpp"

#include "Renderer.hpp"
#include "Platform/GraphicsAPI/Vulkan/Rendering/VulkanVertexArray.hpp"

namespace VX
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VX_CORE_ASSERT(false, "Vertex Array: RendererAPI::None is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::Vulkan:
                return CreateRef<vkclass::VulkanVertexArray>();
                break;
            case RendererAPI::API::DX12:
                VX_CORE_ASSERT(false, "Vertex Array: RendererAPI::DX12 is currently not supported!");
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
                VX_CORE_ASSERT(false, "Vertex Array: RendererAPI::OpenGL is currently not supported!");
                return nullptr;
                break;
            default:
                VX_CORE_ASSERT(false, "Vertex Array: RendererAPI::<API> is not identified!");
                return nullptr;
                break;
        }
        return nullptr;
    }

    void VertexArray::AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer)
    {
        m_vertexBuffers.push_back(vertexBuffer);
        AddApiVertexBuffer(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(Ref<IndexBuffer>& indexBuffer)
    {
        m_indexBuffer = indexBuffer;
        SetApiIndexBuffer(indexBuffer);
    }
}
