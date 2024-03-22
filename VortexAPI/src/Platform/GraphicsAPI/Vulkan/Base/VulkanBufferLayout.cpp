#include "VulkanBufferLayout.hpp"

namespace vkclass
{
//    void VulkanBufferLayout::Set()
//    {
//        
//    }

    void VulkanBufferLayout::Create()
    {
        int stride = 0;
        int offset = 0;
        
        int locationIndex = 0;
        for(const auto& element : Elements)
        {
            VkVertexInputAttributeDescription attribute;
            attribute.binding = 0;
            attribute.location = locationIndex;
            attribute.offset = offset;
            attribute.format = ShaderDataTypeToVulkanFormat(element.Type);
//            VX_CORE_INFO("{0}: offset {1}", element.Name, attribute.offset);
            m_attributeDescrtiptions.push_back(attribute);
            
             offset += ShaderDataTypeSize(element.Type);
             stride += ShaderDataTypeSize(element.Type);
            locationIndex++;
        }
        
        m_bindingDescription.binding = 0;
        m_bindingDescription.stride = stride; // number of bytes from one entry to the next
//        VX_CORE_INFO("Stride: {0}", stride);
        m_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // per-vertex data, TODO: change this for particle system
    }
}
