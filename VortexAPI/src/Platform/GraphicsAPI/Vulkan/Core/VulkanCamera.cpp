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
        
        // TODO: FrameInFlight uniform buffer has two info,
        // so add write descriptor should have two info,
        // but the addbinding just has one argument for that

        // m_descriptor->AddBinding(0, m_uniformBuffer.GetDescriptorInfo());
    }
}
