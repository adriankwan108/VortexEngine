#include "VulkanShader.hpp"

namespace vkclass
{
    VkDevice vkclass::VulkanShader::m_device = VK_NULL_HANDLE;
    vkclass::VulkanCommandManager* vkclass::VulkanShader::m_commandBufferManager = nullptr;

    VulkanShader::VulkanShader(const std::string& vertFilePath, const std::string fragFilePath)
        :m_vertFilePath(vertFilePath),m_fragFilePath(fragFilePath)
    {
        // read file
        std::vector<char> vertShaderCode = VX::Utils::readFile(m_vertFilePath);
        std::vector<char> fragShaderCode = VX::Utils::readFile(m_fragFilePath);
        
        if (vertShaderCode.empty() || fragShaderCode.empty())
        {
            VX_CORE_WARN("Shader files require both .vert & .frag");
            return;
        }
        
        VkShaderModuleCreateInfo vertModuleCI{};
        vertModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertModuleCI.codeSize = vertShaderCode.size();
        vertModuleCI.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());
        if(vkCreateShaderModule(m_device, &vertModuleCI, nullptr, &m_vertModule) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_INFO("Failed to create vert shader module");
            return;
        }
        
        VkShaderModuleCreateInfo fragModuleCI{};
        fragModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragModuleCI.codeSize = fragShaderCode.size();
        fragModuleCI.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());
        if(vkCreateShaderModule(m_device, &fragModuleCI, nullptr, &m_fragModule) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_INFO("Failed to create frag shader module");
            return;
        }
        
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_WARN("VulkanShader: Failed to create pipeline layout!");
            return;;
        }
        
        m_isValid = true;
        // TODO: if not valid, try default shader
    }

    VulkanShader::~VulkanShader()
    {
        if (m_isValid)
        {
            vkDestroyPipeline(m_device, m_pipeline, nullptr);
            vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
            
            vkDestroyShaderModule(m_device, m_vertModule, nullptr);
            vkDestroyShaderModule(m_device, m_fragModule, nullptr);
            VX_CORE_INFO("VulkanShader: Pipeline & layout destroyed");
        }
    }

    void VulkanShader::Init(VkDevice device, vkclass::VulkanCommandManager* commandBufferManager)
    {
        m_device = device;
        m_commandBufferManager = commandBufferManager;
    }

    void VulkanShader::Bind()
    {
        // vkcmdbindpipeline
        m_commandBufferManager->BindPipeline(m_pipeline);
    }

    void VulkanShader::UnBind()
    {
        
    }

    void VulkanShader::CreatePipeline(VkRenderPass renderPass)
    {
        m_pipelineBuilder.SetShaders(m_vertModule, m_fragModule);
        
        // TODO: Set input layout explicitly
        auto bindingDescription = Geometry::Vertex::getBindingDescription();
        auto attributeDesciption = Geometry::Vertex::getAttributeDescriptions();
        m_pipelineBuilder.SetVertexInput(&bindingDescription, &attributeDesciption);
        
        m_pipeline = m_pipelineBuilder.BuildPipeline(m_pipelineLayout, renderPass);
    }

}
