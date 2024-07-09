#include "VulkanShaderLayout.hpp"
#include "Core/VulkanDescriptorManager.hpp"

namespace vkclass
{
    //VulkanVertexShaderLayout::VulkanVertexShaderLayout(VX::VertexShaderLayout layout)
    //{
    //    SetLayout(layout);
    //}

    //void VulkanVertexShaderLayout::SetLayout(VX::VertexShaderLayout layout)
    //{
    //    int stride = 0;
    //    int offset = 0;
    //   
    //    int locationIndex = 0;
    //   
    //    for(const auto& element: layout.Elements)
    //    {
    //        VkVertexInputAttributeDescription attribute{};
    //        attribute.binding = 0;
    //        attribute.location = locationIndex;
    //        attribute.offset = offset;
    //        attribute.format = ShaderDataTypeToVulkanFormat(element.Type);
    //        m_attributeDescriptions.push_back(attribute);
    //   
    //        offset += element.Size;

    //        // VX_CORE_TRACE("attribute {0}: location {1}, offset {2}, size:{3}", element.Name, attribute.location, attribute.offset, element.Size);
    //        locationIndex++;
    //    }
    //   
    //    m_bindingDescription.binding = 0;
    //    m_bindingDescription.stride = layout.GetStride(); // number of bytes from one entry to the next
    //    m_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // per-vertex data, TODO: change this for particle system
    //}
}
