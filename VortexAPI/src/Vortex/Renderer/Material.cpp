#include "Material.hpp"

#include "Renderer.hpp"
#include "Platform/GraphicsAPI/Vulkan/Rendering/VulkanMaterial.hpp"

namespace VX
{
    Ref<Material> Material::Create(Ref<ShaderEffect> shaderEffect)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::API::Vulkan:
            return CreateRef<vkclass::VulkanMaterial>(shaderEffect);
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
