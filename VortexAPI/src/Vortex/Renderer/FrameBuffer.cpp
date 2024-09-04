#include "VortexPCH.hpp"
#include "FrameBuffer.hpp"
#include "Renderer.hpp"
#include "Platform/GraphicsAPI/Vulkan/Rendering/VulkanFrameBuffer.hpp"

namespace VX
{
    Ref<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::API::Vulkan:
            return CreateRef<vkclass::VulkanFrameBuffer>(spec);
            break;
        case RendererAPI::API::DX12:
            VX_CORE_ASSERT(false, "RendererAPI::DX12 is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            VX_CORE_ASSERT(false, "RendererAPI::OpenGL is currently not supported!");
            return nullptr;
            break;
        default:
            VX_CORE_ASSERT(false, "RendererAPI::<API> is not identified!");
            return nullptr;
            break;
        }
        return nullptr;
    }
}
