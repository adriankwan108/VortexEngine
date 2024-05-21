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
        
        VX::VertexShaderLayout ImguiLayout = {
            {VX::ShaderDataType::Float2, "pos"},
            {VX::ShaderDataType::Float2, "uv"},
            {VX::ShaderDataType::U32, "col"}
        };

    private:
        VX::Ref<VulkanShader> m_fontShader;
        VX::Ref<VulkanTexture2D> m_fontTexture;
        
        VX::Ref<VulkanVertexArray> m_vertexArray;
        VX::Ref<VulkanVertexBuffer> m_vertexBuffer;
        VX::Ref<VulkanIndexBuffer> m_indexBuffer;
        
        int32_t vertexCount = 0;
        int32_t indexCount = 0;
        
        // Update vertex and index buffer containing the imGui elements when required
        void updateBuffers();
    };
}
