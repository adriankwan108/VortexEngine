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
#include "Core/VulkanDescriptorManager.hpp"
#include "VulkanRenderPass.hpp"
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
        case VX::ShaderDataType::Float2:   
            VX_CORE_TRACE("Format: VK_FORMAT_R32G32_SFLOAT");
            return VK_FORMAT_R32G32_SFLOAT;
        case VX::ShaderDataType::Float3:   
            VX_CORE_TRACE("Format: VK_FORMAT_R32G32B32_SFLOAT");
            return VK_FORMAT_R32G32B32_SFLOAT;
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

    /*
    * VulkanShader serves for loading, reflecting, and storing the necessary information
    */
    class VulkanShader : public VX::Shader
    {
    public:
        VulkanShader(const std::string& name, const std::string& filePath, VX::ShaderStage stage);
        ~VulkanShader();
        
    public:
        // Vulkan Specific funcs
        static VkDevice s_device;
        static void Init(VkDevice device);
        
        void ClearModule();
        const VkShaderModule& GetModule();
        
    protected:
        virtual bool read() override;
        virtual void reflect() override;

    private:
        VkShaderModule m_module = VK_NULL_HANDLE;
        bool isModuleLoaded = false;
        void loadModule();
    };

    static VkShaderStageFlags VulkanShaderStage(VX::ShaderStage stage)
    {
        switch (stage)
        {
        case VX::ShaderStage::Vertex:   return VK_SHADER_STAGE_VERTEX_BIT;
        case VX::ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
        default: return VK_SHADER_STAGE_ALL;
        }
    }

    static VkShaderStageFlagBits VulkanShaderStageFlagBits(VX::ShaderStage stage)
    {
        switch (stage)
        {
        case VX::ShaderStage::Vertex:   return VK_SHADER_STAGE_VERTEX_BIT;
        case VX::ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
        default: return VK_SHADER_STAGE_ALL;
        }
    }

    /*
    * VulkanShaderPass serves for combining & transforming shader infos into essential Vulkan's layouts or formats
    * for the usage of Vertex Shader creation, pipelines creation, ...
    */
    class VulkanShaderPass : public VX::ShaderPass
    {
    public:
        VulkanShaderPass();
        virtual ~VulkanShaderPass() override;
        virtual void Prepare() override;
    public:
        // Vulkan Specific functions
        const VkVertexInputBindingDescription& GetVertexInputBinding() { return m_bindingDescription; }
        const std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributes() { return m_attributeDescriptions; }
        const VkPipelineLayout& GetPipelineLayout() { return m_pipelineLayout; }
        std::vector<VX::Ref<VulkanShader>> GetVulkanShaders();
        static void Init(VkDevice device);
        
    private:
        VkVertexInputBindingDescription m_bindingDescription{};
        std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
        std::vector<DescriptorLayoutHandle> m_descriptorLayoutHandles;
        // std::vector<std::pair<VkPushConstantRange, void*>> m_pushConstants;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
        
        static VkDevice s_device;

        void prepareVertexShader    (VX::Ref<VX::Shader> shader);
        void prepareFragmentShader  (VX::Ref<VX::Shader> shader);
    };

    /*
    * VulkanShaderEffect is the actual built version of VulkanShaderPass, which requires several renderpass
    */
    class VulkanShaderEffect : public VX::ShaderEffect
    {
    public:
        VulkanShaderEffect(VX::Ref<VX::ShaderPass> shaderPass);
        virtual ~VulkanShaderEffect() override;

        virtual void Build() override;
        virtual void Bind() override;

        static void Init(VkDevice device, VulkanRenderPassManager* renderPassManager);
    public:
        VX::Ref<VulkanShaderPass> GetVulkanShaderPass() { return m_VulkanShaderPass; }
        
    private:
        static VkDevice s_device;
        static VulkanRenderPassManager* s_renderPassManager;

        VX::Ref<VulkanShaderPass> m_VulkanShaderPass;
        // request renderpass
        // request resources
        VkPipeline m_pipeline;
        VulkanPipelineBuilder m_builder;
    };
}
