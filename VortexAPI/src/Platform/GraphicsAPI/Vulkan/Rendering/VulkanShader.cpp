#include "VulkanShader.hpp"
#include "Core/VulkanCamera.hpp"

namespace vkclass
{
    VkDevice vkclass::VulkanShader::m_device = VK_NULL_HANDLE;
    vkclass::VulkanCommandManager* vkclass::VulkanShader::m_commandBufferManager = nullptr;
    VkRenderPass vkclass::VulkanShader::s_RenderPass = VK_NULL_HANDLE;

    VulkanShader::VulkanShader(const std::string& name, const std::string& vertFilePath, const std::string fragFilePath)
        : m_Name(name), m_vertFilePath(vertFilePath),m_fragFilePath(fragFilePath)
    {
        // read file
        std::vector<char> vertShaderCode = VX::Utils::readFile(m_vertFilePath);
        std::vector<char> fragShaderCode = VX::Utils::readFile(m_fragFilePath);
        
        if (vertShaderCode.empty() || fragShaderCode.empty())
        {
            VX_CORE_WARN("Vulkan Shader: Read failed. Shader files require both non-empty .vert & .frag");
            return;
        }
        
        VkShaderModuleCreateInfo vertModuleCI{};
        vertModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertModuleCI.codeSize = vertShaderCode.size();
        vertModuleCI.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());
        if(vkCreateShaderModule(m_device, &vertModuleCI, nullptr, &m_vertModule) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_INFO("Vulkan Shader: Failed to create vert shader module");
            return;
        }
        
        VkShaderModuleCreateInfo fragModuleCI{};
        fragModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragModuleCI.codeSize = fragShaderCode.size();
        fragModuleCI.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());
        if(vkCreateShaderModule(m_device, &fragModuleCI, nullptr, &m_fragModule) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_INFO("Vulkan Shader: Failed to create frag shader module");
            return;
        }
        
        m_isValid = true;
        // TODO: if not valid, use default shader (guaranteed)
        
        
    }

    VulkanShader::~VulkanShader()
    {
        if (m_isValid)
        {
            if(m_pipeline != VK_NULL_HANDLE)
            {
                vkDestroyPipeline(m_device, m_pipeline, nullptr);
            }
            
            if(m_pipelineLayout != VK_NULL_HANDLE)
            {
                vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
            }
            VX_CORE_INFO("VulkanShader: Pipeline & layout destroyed");
        }
    }

    void VulkanShader::Init(VkDevice device, vkclass::VulkanCommandManager* commandBufferManager, VkRenderPass renderPass)
    {
        m_device = device;
        m_commandBufferManager = commandBufferManager;
        s_RenderPass = renderPass;
    }

    void VulkanShader::Bind() const
    {
        // bind 
        m_commandBufferManager->BindPipeline(m_pipeline);
    }

    void VulkanShader::UnBind() const
    {
        
    }

    void VulkanShader::SetVertexLayout(VX::VertexShaderLayout layout)
    {
        m_vertexLayout.SetLayout(layout);
    }

    
    void VulkanShader::SetGlobalLayout(int binding, std::shared_ptr<VX::OrthographicCamera> camera)
    {
        // TODO: don't use push back => use insert
        m_descriptorSetLayouts.push_back( std::static_pointer_cast<VulkanCamera>(camera)->GetDescriptor()->GetDescriptorSetLayout());
    }

    void VulkanShader::SetPassLayout(int binding, VX::UniformShaderLayout layout)
    {
         
    }
    void VulkanShader::SetMaterialLayout(int binding, VX::UniformShaderLayout layout)
    {
        
    }

    void VulkanShader::SetObjectLayout(int binding, VX::UniformShaderLayout layout)
    {
        
    }

    void VulkanShader::Prepare()
    {
        // globalSetLayout = descriptorlayoutbuilder.build();
        // globalSet = DescriptorManager.Allocate(globalSetLayout);
        
        // descriptorWriter.UpdateSet(globalSet);

        VX_CORE_TRACE("Vulkan Shader: Preparing pipeline...");
        VulkanPipelineBuilder builder;
        builder.SetShaders(m_vertModule, m_fragModule);
        builder.SetVertexInput(m_vertexLayout.GetBinding(), m_vertexLayout.GetAttributes());
        
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        // pipelineLayoutInfo.setLayoutCount = 0; // Optional
        // pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_descriptorSetLayouts.size()); // Optional
        pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data(); // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
        if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_WARN("VulkanShader: Failed to create pipeline layout!");
            return;;
        }
        
        // build operation
        m_pipeline = builder.BuildPipeline(m_pipelineLayout, s_RenderPass);
        
        // destroy unused resources
        vkDestroyShaderModule(m_device, m_vertModule, nullptr);
        vkDestroyShaderModule(m_device, m_fragModule, nullptr);
        
        VX_CORE_TRACE("Vulkan Shader: Pipeline set");
    }
}
