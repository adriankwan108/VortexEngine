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
    //class VulkanShader : public VX::Shader
    //{
    //public:
    //    VulkanShader(const std::string& name, const std::string& vertFilePath, const std::string fragFilePath);
    //    ~VulkanShader();
    //    
    //    virtual void Bind() override;
    //    virtual void UnBind() const override;
    //    
    //    virtual void SetVertexLayout(VX::VertexShaderLayout layout) override;
    //    
    //    virtual void SetGlobalLayout    (int binding, VX::UniformShaderLayout layout) override;
    //    virtual void SetPassLayout      (int binding, VX::UniformShaderLayout layout) override;
    //    virtual void SetTexture         (VX::Ref<VX::Texture2D> texture )             override;
    //    virtual void SetObjectLayout    (int binding, VX::UniformShaderLayout layout) override;
    //    
    //    virtual void Prepare() override;
    //    
    //    virtual const std::string& GetName() const override { return m_Name; }
    //    
    //public:
    //    const VkShaderModule& VertModule = m_vertModule;
    //    const VkShaderModule& FragModule = m_fragModule;
    //    const VkPipeline& Pipeline = m_pipeline;
    //    const VkPipelineLayout& PipelineLayout = m_pipelineLayout;
    //
    //    bool& Valid = m_isValid;
    //    
    //public:
    //    void SetPushConstant(VkShaderStageFlags stage, uint32_t size, void* pValue);
    //    
    //    static void Init(VkDevice device, vkclass::VulkanCommandManager* commandBufferManager, VkRenderPass renderPass);
    //    
    //private:
    //    static VkDevice m_device;
    //    static vkclass::VulkanCommandManager* m_commandBufferManager;
    //    static VkRenderPass s_RenderPass;
    //    
    //    // props
    //    uint32_t m_RendererID = 0;
    //    std::string m_Name;
    //    
    //    std::string m_vertFilePath;
    //    std::string m_fragFilePath;
    //    
    //    vkclass::VulkanVertexShaderLayout m_vertexLayout;
    //    
    //    std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
    //    std::vector<std::pair<VkPushConstantRange, void*>> m_pushConstants;

    //    VX::Ref<VulkanDescriptor> m_textureDescriptor;
    //    
    //    VkPipelineLayout m_pipelineLayout;
    //    VkPipeline m_pipeline;
    //    
    //    VX::Ref<VulkanTexture2D> m_texture;

    //    // private var
    //    VkShaderModule m_vertModule = VK_NULL_HANDLE;
    //    VkShaderModule m_fragModule = VK_NULL_HANDLE;
    //    
    //    bool m_isValid = false;

    //private:
    //    void reflect(const std::vector<uint32_t>& data);
    //};

    static VkFormat ShaderDataTypeToVulkanFormat(VX::ShaderDataType type)
    {
        switch (type)
        {
        case VX::ShaderDataType::None:     return VK_FORMAT_UNDEFINED;
        case VX::ShaderDataType::Float:    return VK_FORMAT_R32_SFLOAT;
        case VX::ShaderDataType::Float2:   return VK_FORMAT_R32G32_SFLOAT;
        case VX::ShaderDataType::Float3:   return VK_FORMAT_R32G32B32_SFLOAT;
        case VX::ShaderDataType::Float4:   return VK_FORMAT_R32G32B32A32_SFLOAT;
        case VX::ShaderDataType::Mat3:     return VK_FORMAT_R32G32B32_SFLOAT;
        case VX::ShaderDataType::Mat4:     return VK_FORMAT_R32G32B32A32_SFLOAT;
        case VX::ShaderDataType::Int:      return VK_FORMAT_R32_SINT;
        case VX::ShaderDataType::Int2:     return VK_FORMAT_R32G32_SINT;
        case VX::ShaderDataType::Int3:     return VK_FORMAT_R32G32B32_SINT;
        case VX::ShaderDataType::Int4:     return VK_FORMAT_R32G32B32A32_SINT;
        case VX::ShaderDataType::Bool:     return VK_FORMAT_R8_UINT;
        case VX::ShaderDataType::U32:      return VK_FORMAT_R8G8B8A8_UNORM;
        default: VX_CORE_ASSERT(false, "Layout: Unknown Shader data type.");
        }
    }

    class VulkanShader : public VX::Shader
    {
    public:
        VulkanShader(const std::string& name, const std::string& filePath, VX::ShaderStage stage);
        ~VulkanShader();
        static void Init(VkDevice device);
    private:
        void reflect(const std::vector<uint32_t>& data);

    private:
        VkShaderModule m_module = VK_NULL_HANDLE;
        static VkDevice s_device;
    };

    class VulkanShaderPass : public VX::ShaderPass
    {
    public:
        VulkanShaderPass();
        virtual void Prepare() override;

    private:
        VkVertexInputBindingDescription m_bindingDescription{};
        std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
    };

    class VulkanShaderEffect : public VX::ShaderEffect
    {
    public:
        VulkanShaderEffect(VX::Ref<VX::ShaderPass> shaderPass);

        virtual void Build() override;
    private:

    };
}
