#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"

#include "Renderer/Buffer.hpp"

namespace vkclass
{
    class VulkanVertexShaderLayout
    {
    public:
        VulkanVertexShaderLayout() = default;
        VulkanVertexShaderLayout(VX::VertexShaderLayout layout);
        ~VulkanVertexShaderLayout() = default;
        
        void SetLayout(VX::VertexShaderLayout layout);
        
        const VkVertexInputBindingDescription& GetBinding() const { return m_bindingDescription; }
        const std::vector<VkVertexInputAttributeDescription>& GetAttributes() const { return m_attributeDescriptions; }
        uint32_t GetStride() const { return m_stride; }
        
    private:
        VkVertexInputBindingDescription m_bindingDescription{};
        std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
        uint32_t m_stride = 0;
    };

class VulkanUniformLayout
{
public:
    VulkanUniformLayout();
    ~VulkanUniformLayout() = default;
    
    void SetLayout(VX::UniformShaderLayout layout);
    VkDescriptorSetLayout GetDescripotSetLayout() const { return m_descriptorSetLayout; }
private:
    VkDescriptorSetLayout& m_descriptorSetLayout;
};

    static VkFormat ShaderDataTypeToVulkanFormat(VX::ShaderDataType type)
    {
        switch (type)
        {
            case VX::ShaderDataType::None:     return VK_FORMAT_UNDEFINED;
            case VX::ShaderDataType::Float:    return VK_FORMAT_R32_SFLOAT;
            case VX::ShaderDataType::Float2:   
                VX_CORE_TRACE("Format: VK_FORMAT_R32G32_SFLOAT");
                return VK_FORMAT_R32G32_SFLOAT;
            case VX::ShaderDataType::Float3:   
                VX_CORE_TRACE("Format: VK_FORMAT_R32G32B32_SFLOAT");
                return VK_FORMAT_R32G32B32_SFLOAT;
            case VX::ShaderDataType::Float4:   return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VX::ShaderDataType::Mat3:     return VK_FORMAT_R32G32B32_SFLOAT;
            case VX::ShaderDataType::Mat4:     return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VX::ShaderDataType::Int:      return VK_FORMAT_R32_SINT;
            case VX::ShaderDataType::Int2:     return VK_FORMAT_R32G32_SINT;
            case VX::ShaderDataType::Int3:     return VK_FORMAT_R32G32B32_SINT;
            case VX::ShaderDataType::Int4:     return VK_FORMAT_R32G32B32A32_SINT;
            case VX::ShaderDataType::Bool:     return VK_FORMAT_R8_UINT;
            default: VX_CORE_ASSERT(false, "Layout: Unknown Shader data type.");
        }
    }
}
