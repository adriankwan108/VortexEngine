#include "VulkanShaderLayout.hpp"

namespace vkclass
{
    VulkanShaderLayout::VulkanShaderLayout(VX::BufferLayout layout)
    {
        SetLayout(layout);
    }

    void VulkanShaderLayout::SetLayout(VX::BufferLayout layout)
    {
        int stride = 0;
        int offset = 0;
       
        int locationIndex = 0;
       
        for(const auto& element: layout.Elements)
        {
            VkVertexInputAttributeDescription attribute;
            attribute.binding = 0;
            attribute.location = locationIndex;
            attribute.offset = offset;
            attribute.format = ShaderDataTypeToVulkanFormat(element.Type);
            
            m_attributeDescriptions.push_back(attribute);
       
            offset += ShaderDataTypeSize(element.Type);
            stride += ShaderDataTypeSize(element.Type);
            locationIndex++;
        }
       
        m_bindingDescription.binding = 0;
        m_bindingDescription.stride = stride; // number of bytes from one entry to the next
        m_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // per-vertex data, TODO: change this for particle system
    }
}
