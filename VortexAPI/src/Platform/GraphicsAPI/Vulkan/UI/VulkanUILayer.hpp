#pragma once
#include "VortexPCH.hpp"
#include "Vortex/UI/UILayer.hpp"
#include <imgui.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

#include "Platform/GraphicsAPI/Vulkan/Rendering/VulkanShader.hpp"
#include "Platform/GraphicsAPI/Vulkan/Rendering/VulkanTexture.hpp"
#include "Platform/GraphicsAPI/Vulkan/Rendering/VulkanVertexArray.hpp"
#include "Core/VulkanBuffer.hpp"

namespace vkclass
{
    class VulkanUILayer : public VX::UILayer
    {
    public:
        VulkanUILayer();
        ~VulkanUILayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(VX::Timestep ts) override {};
        virtual void OnEvent(VX::Event& event) override;

        virtual void OnUpdateStart() override;
        virtual void OnUpdateEnd() override;
        
        struct PushConstBlock {
            glm::vec2 scale;
            glm::vec2 translate;
        } FontPushConstant;
        
    private:
        VX::Ref<VulkanShader> m_fontShader;
        VX::Ref<VulkanTexture2D> m_fontTexture;
        
        VX::Ref<VX::VertexArray> m_vertexArray;
        VX::Ref<VX::VertexBuffer> m_vertexBuffer;
        VX::Ref<VX::IndexBuffer> m_indexBuffer;
        
        int32_t vertexCount = 0;
        int32_t indexCount = 0;
        
        // Update vertex and index buffer containing the imGui elements when required
        void updateBuffers();
    };
}
