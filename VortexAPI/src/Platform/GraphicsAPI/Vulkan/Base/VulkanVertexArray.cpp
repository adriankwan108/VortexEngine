#include "VulkanVertexArray.hpp"

namespace vkclass
{
    VulkanVertexArray::VulkanVertexArray()
    {
        // glcreatevertexarray(1, &rendererid)
    }

    VulkanVertexArray::~VulkanVertexArray()
    {
        
    }

    void VulkanVertexArray::Bind() const
    {
        // glbindvertexarray(rendererid)
    }

    void VulkanVertexArray::Unbind() const
    {
        // hlbindvertexarray(0)
    }

    void VulkanVertexArray::AddVertexBuffer(std::shared_ptr<VX::VertexBuffer>& vertexBuffer)
    {
        // vertexbuffer->Bind
        // uint32_t index = 0;
        // get layout
        // for element in layout
        //    glenableVertexAttributeArray(index)
        //    glvertexAttrtibutePointer(
        //        element.componentCount,
        //        Change datatype to vulkan format,
        //        normal,
        //        getStride,
        //        offset
        //    )
        //    index++
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void VulkanVertexArray::SetIndexBuffer(std::shared_ptr<VX::IndexBuffer>& indexBuffer)
    {
        
    }
}
