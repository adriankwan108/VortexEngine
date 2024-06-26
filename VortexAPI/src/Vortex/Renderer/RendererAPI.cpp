#include "RendererAPI.hpp"

#include "Platform/GraphicsAPI/Vulkan/Rendering/VulkanRendererAPI.hpp"

namespace VX
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch ( s_API )
        {
        case RendererAPI::API::None:
            VX_CORE_ASSERT(false, "RendererAPI::None is not supported");
            return nullptr;
            break;
        case RendererAPI::API::Vulkan:
            return CreateScope<vkclass::VulkanRendererAPI>();
            break;
        case RendererAPI::API::DX12:
            VX_CORE_ASSERT(false, "RendererAPI::DX12 is not supported");
            return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            VX_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported");
            return nullptr;
            break;
        default:
            VX_CORE_ASSERT(false, "RendererAPI::<API> is not identified");
            return nullptr;
            break;
        }
    }
}
