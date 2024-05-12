#include "UILayer.hpp"
#include "VortexPCH.hpp"

#include "Renderer/Renderer.hpp"

#include "Platform/GraphicsAPI/Vulkan/UI/VulkanUILayer.hpp"

// Actually should have made an abstract interface for choosing ui library
// but I go straight for ImGui, unless I have got super rich and extreme plenty of time

namespace VX
{
    UILayer::UILayer()
        : Layer("UserInterface")
    {
    }

    UILayer* UILayer::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::API::Vulkan:
            return new vkclass::VulkanUILayer();
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