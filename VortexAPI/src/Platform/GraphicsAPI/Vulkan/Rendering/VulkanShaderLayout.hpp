#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"

#include "Renderer/Buffer.hpp"

namespace vkclass
{
    class VulkanShaderLayout
    {
    public:
        VulkanShaderLayout() = default;
        VulkanShaderLayout(VX::BufferLayout layout);
        ~VulkanShaderLayout() = default;
        
        void SetLayout(VX::BufferLayout layout);
        
        const VkVertexInputBindingDescription& GetBinding() const { return m_bindingDescription; }
        const std::vector<VkVertexInputAttributeDescription>& GetAttributes() const { return m_attributeDescriptions; }
        uint32_t GetStride() const { return m_stride; }
        
    private:
        VkVertexInputBindingDescription m_bindingDescription;
        std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
        uint32_t m_stride = 0;
    };

    static VkFormat ShaderDataTypeToVulkanFormat(VX::ShaderDataType type)
    {
        switch (type)
        {
            case VX::ShaderDataType::None:     return VK_FORMAT_UNDEFINED;
            case VX::ShaderDataType::Float:    return VK_FORMAT_R32_SFLOAT;
            case VX::ShaderDataType::Float2:   return VK_FORMAT_R32G32_SFLOAT;
            case VX::ShaderDataType::Float3:   return VK_FORMAT_R32G32B32_SFLOAT;
            case VX::ShaderDataType::Float4:   return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VX::ShaderDataType::Mat3:     return VK_FORMAT_R32G32B32_SFLOAT;
            case VX::ShaderDataType::Mat4:     return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VX::ShaderDataType::Int:      return VK_FORMAT_R32_SINT;
            case VX::ShaderDataType::Int2:     return VK_FORMAT_R32G32_SINT;
            case VX::ShaderDataType::Int3:     return VK_FORMAT_R32G32B32_SINT;
            case VX::ShaderDataType::Int4:     return VK_FORMAT_R32G32B32A32_SINT;
            case VX::ShaderDataType::Bool:     return VK_FORMAT_R8_UINT;
        }
    }

    static uint32_t ShaderDataTypeSize(VX::ShaderDataType type)
    {
        switch (type)
        {
            case VX::ShaderDataType::Float:    return 4;
            case VX::ShaderDataType::Float2:   return 4 * 2;
            case VX::ShaderDataType::Float3:   return 4 * 3;
            case VX::ShaderDataType::Float4:   return 4 * 4;
            case VX::ShaderDataType::Mat3:     return 4 * 3 * 3;
            case VX::ShaderDataType::Mat4:     return 4 * 4 * 4;
            case VX::ShaderDataType::Int:      return 4;
            case VX::ShaderDataType::Int2:     return 4 * 2;
            case VX::ShaderDataType::Int3:     return 4 * 3;
            case VX::ShaderDataType::Int4:     return 4 * 4;
            case VX::ShaderDataType::Bool:     return 1;
            default:                       VX_CORE_ASSERT(false, "Unknown Shader data type.");
        }
    }
}
