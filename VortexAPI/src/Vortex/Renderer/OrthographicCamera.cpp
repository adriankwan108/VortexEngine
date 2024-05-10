#include "OrthographicCamera.hpp"
#include "VortexPCH.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.hpp"
#include "Platform/GraphicsAPI/Vulkan/Core/VulkanCamera.hpp"

namespace VX
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        m_ProjectionMatrix[1][1] *= m_isYFlip? -1:1;
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

        m_ProjectionMatrix[1][1] *= m_isYFlip? -1:1;
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetYFlip(bool isYFlip)
    {
        // flip from left-hand to right-hand system
        // thus, all shaders, calculation are in left-hand, the graphic engine internally using right-hand
        m_isYFlip = isYFlip;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position* (1.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
        
        m_ViewMatrix = glm::inverse(transform);
        
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        Update();
    }

    OrthographicCamera* OrthographicCamera::Create(float left, float right, float bottom, float top)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            VX_CORE_ASSERT(false, "RendererAPI::None is not supported");
            return nullptr;
            break;
        case RendererAPI::API::Vulkan:
            return new vkclass::VulkanCamera(left, right, bottom, top);
            break;
        case RendererAPI::API::DX12:
            VX_CORE_ASSERT(false, "RendererAPI::DX12 is not supported");
            return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            VX_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported");
            return nullptr;
            break;
        default:
            VX_CORE_ASSERT(false, "RendererAPI::<API> is not identified");
            return nullptr;
            break;
        }
    }
}
