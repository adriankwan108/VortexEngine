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
        // update uniform buffer
        camera_vp.view = GetViewMatrix();
        camera_vp.proj = GetProjectionMatrix();
        m_uniformBuffer.Update(&camera_vp, sizeof(camera_vp));
    }

    void VulkanCamera::setDescriptor()
    {
//        VX_CORE_INFO("VulkanCamera: Setting Descriptor...");
        m_descriptor = DescriptorManager::CreateDescriptor();
        m_descriptor->AddBinding(0, &m_uniformBuffer);
        m_descriptor->Build();
//        VX_CORE_INFO("VulkanCamera: Descriptor is set.");
        
        GlobalDescriptor::SetDescriptor(m_descriptor);
    }
}
