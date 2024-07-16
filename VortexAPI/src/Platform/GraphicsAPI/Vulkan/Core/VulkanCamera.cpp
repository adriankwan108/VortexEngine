#include "VulkanCamera.hpp"
// #include <glm/gtx/string_cast.hpp>

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

        /*camera_vp.view = glm::lookAt(
            glm::vec3(2.0f, 2.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        camera_vp.proj = glm::perspective(glm::radians(45.0f), 1920 / (float)1080, 0.1f, 10.0f);*/

        m_uniformBuffer.Update(&camera_vp, sizeof(camera_vp));
    }

    void VulkanCamera::setDescriptor()
    {
        /*m_descriptor = DescriptorManager::CreateDescriptor();
        m_descriptor->SetStage(VK_SHADER_STAGE_VERTEX_BIT);
        m_descriptor->AddBinding(0, &m_uniformBuffer);
        m_descriptor->Build();

        GlobalDescriptor::SetDescriptor(m_descriptor);*/
    }
}
