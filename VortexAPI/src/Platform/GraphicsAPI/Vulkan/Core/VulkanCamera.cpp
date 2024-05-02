#include "VulkanCamera.hpp"

namespace vkclass
{
    VulkanDevice* VulkanCamera::m_device = nullptr;

    VulkanCamera::~VulkanCamera()
    {
        
    }

    void VulkanCamera::Init(VulkanDevice *device)
    {
        m_device = device;
    }

    void VulkanCamera::Update()
    {
        
    }

    void VulkanCamera::setDescriptor()
    {
        m_descriptor = DescriptorManager::CreateDescriptor();
        DescriptorLayoutBuilder builder;
        builder.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        m_descriptor->SetDescriptorSetLayout(builder.Build(m_device->LogicalDevice,  VK_SHADER_STAGE_VERTEX_BIT));
        
        // descriptor manager should allocate based on frames in flight
        // descriptorSet = DescriptorManager.Allocate();
        
        DescriptorWriter writer;
        writer.WriteBuffer(0, <#VkBuffer buffer#>, <#size_t size#>, <#size_t offset#>, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        writer.UpdateSet(m_device, <#VkDescriptorSet set#>)
    }
}
