#pragma once
#include "VortexPCH.hpp"
#include "Vortex/Utils.hpp"

#include "vulkan/vulkan.h"

#include "Vortex/Geometry/Geometry.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

#include "Core/VulkanInitializer.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanCommandManager.hpp"
#include "VulkanPipelineBuilder.hpp"
#include "VulkanShaderLayout.hpp"
#include "VulkanTexture.hpp"

namespace vkclass
{
    class VulkanShader : public VX::Shader
    {
    public:
        VulkanShader(const std::string& name, const std::string& vertFilePath, const std::string fragFilePath);
        ~VulkanShader();
        
        virtual void Bind() override;
        virtual void UnBind() const override;
        
        virtual void SetVertexLayout(VX::VertexShaderLayout layout) override;
        
        virtual void SetGlobalLayout    (int binding, VX::UniformShaderLayout layout) override;
        virtual void SetPassLayout      (int binding, VX::UniformShaderLayout layout) override;
        virtual void SetTexture         (VX::Ref<VX::Texture2D> texture )             override;
        virtual void SetObjectLayout    (int binding, VX::UniformShaderLayout layout) override;
        
        virtual void Prepare() override;
        
        virtual const std::string& GetName() const override { return m_Name; }
        
    public:
        const VkShaderModule& VertModule = m_vertModule;
        const VkShaderModule& FragModule = m_fragModule;
        const VkPipeline& Pipeline = m_pipeline;
        const VkPipelineLayout& PipelineLayout = m_pipelineLayout;
    
        bool& Valid = m_isValid;
        
    public:
        void SetPushConstant(VkShaderStageFlags stage, uint32_t size, void* pValue);
        
        static void Init(VkDevice device, vkclass::VulkanCommandManager* commandBufferManager, VkRenderPass renderPass);
        
    private:
        static VkDevice m_device;
        static vkclass::VulkanCommandManager* m_commandBufferManager;
        static VkRenderPass s_RenderPass;
        
        // props
        uint32_t m_RendererID = 0;
        std::string m_Name;
        
        std::string m_vertFilePath;
        std::string m_fragFilePath;
        
        vkclass::VulkanVertexShaderLayout m_vertexLayout;
        
        std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
        std::vector<std::pair<VkPushConstantRange, void*>> m_pushConstants;

        VX::Ref<VulkanDescriptor> m_textureDescriptor;
        
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
        
        VX::Ref<VulkanTexture2D> m_texture;

        // private var
        VkShaderModule m_vertModule = VK_NULL_HANDLE;
        VkShaderModule m_fragModule = VK_NULL_HANDLE;
        
        bool m_isValid = false;
    };
}
