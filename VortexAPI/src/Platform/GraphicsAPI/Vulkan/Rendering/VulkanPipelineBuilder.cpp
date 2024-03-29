#include "VulkanPipelineBuilder.hpp"

namespace vkclass
{
    VkDevice vkclass::VulkanPipelineBuilder::m_device = VK_NULL_HANDLE;

    VulkanPipelineBuilder::VulkanPipelineBuilder()
    {
        // Clear();
    }

    void VulkanPipelineBuilder::Init(VkDevice device)
    {
        m_device = device;
    }

    VkPipeline VulkanPipelineBuilder::BuildPipeline(VkPipelineLayout layout, VkRenderPass renderPass)
    {
        if (renderPass == VK_NULL_HANDLE || layout == VK_NULL_HANDLE)
        {
            VX_CORE_ASSERT(false, "Pipeline Builder: Building with null render pass...");
        }
        VkGraphicsPipelineCreateInfo pipelineInfo = vkclass::initializers::pipelineCreateInfo(layout, renderPass);
        pipelineInfo.subpass = 0; // we will need a graphics pipeline for every subpass if we intend to use it multiple times within a single render pass or different incompatible render passes
        
        // shaders
        pipelineInfo.stageCount = static_cast<uint32_t>(m_shaderStages.size());
        pipelineInfo.pStages = m_shaderStages.data();
        
        // fixed functions
        pipelineInfo.pVertexInputState = &m_vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &m_inputAssembly;
        pipelineInfo.pViewportState = &m_viewportState;
        pipelineInfo.pRasterizationState = &m_rasterizer;
        pipelineInfo.pMultisampleState = &m_multisampling; // this should be optional
        pipelineInfo.pDepthStencilState = nullptr; // this should be optional
        pipelineInfo.pColorBlendState = &m_colorBlending;
        
        VkPipelineDynamicStateCreateInfo dynamicaStateInfo = vkclass::initializers::pipelineDynamicStateCreateInfo(m_dynamicStates);
        pipelineInfo.pDynamicState = &dynamicaStateInfo;
        
        VkPipeline newPipeline;
        VK_CHECK_RESULT(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline));
        return newPipeline;
    }

    void VulkanPipelineBuilder::Clear()
    {
        // reset all parameters to zero
        m_vertexInputInfo = vkclass::initializers::pipelineVertexInputStateCreateInfo({},{});
        m_inputAssembly = vkclass::initializers::pipelineInputAssemblyStateCreateInfo(DefaultTopology, 0, VK_FALSE);
        m_viewportState = vkclass::initializers::pipelineViewportStateCreateInfo(1, 1);
        m_rasterizer = vkclass::initializers::pipelineRasterizationStateCreateInfo(DefaultPolygonMode, DefaultCullMode, DefaultFrontFace);
        
        // default no multi-sampling
        m_multisampling = vkclass::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
        m_multisampling.sampleShadingEnable = VK_FALSE;
        m_multisampling.minSampleShading = 1.0f; // Optional
        m_multisampling.pSampleMask = nullptr; // Optional
        m_multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        m_multisampling.alphaToOneEnable = VK_FALSE; // Optional
        
//        m_depthStencil = vkclass::initializers::pipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp);
        
        EnableBlending(false);
        m_colorBlending = vkclass::initializers::pipelineColorBlendStateCreateInfo(1, &m_colorBlendAttachment);
        m_colorBlending.logicOpEnable = VK_FALSE;
        m_colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        m_colorBlending.blendConstants[0] = 0.0f; // Optional
        m_colorBlending.blendConstants[1] = 0.0f; // Optional
        m_colorBlending.blendConstants[2] = 0.0f; // Optional
        m_colorBlending.blendConstants[3] = 0.0f; // Optional
        
        m_shaderStages.clear();
        
        EnableDepthClamp(false);
        EnableDepthBias(false);
        VX_CORE_INFO("PipelineBuilder: Reset");
    }

    void VulkanPipelineBuilder::SetShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader)
    {
        Clear();
        
        // it's possible to combine multiple fragment shaders into a single shader module and use different entry points to differentiate between their behaviors. In this case we'll stick to the standard main, however.
        
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertexShader;
        vertShaderStageInfo.pName = "main";
        // pSpecializationInfo allows you to specify values for shader constant
        m_shaderStages.push_back(vertShaderStageInfo);
        
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragmentShader;
        fragShaderStageInfo.pName = "main";
        
        m_shaderStages.push_back(fragShaderStageInfo);
        VX_CORE_INFO("Pipeline Builder: Shaders set.");
    }

    void VulkanPipelineBuilder::SetVertexInput(const VkVertexInputBindingDescription& bindings, const std::vector<VkVertexInputAttributeDescription>& attributes)
    {
        m_vertexInputInfo.vertexBindingDescriptionCount = 1;
        m_vertexInputInfo.pVertexBindingDescriptions = &bindings;
        m_vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
        m_vertexInputInfo.pVertexAttributeDescriptions = attributes.data();
        
        VX_CORE_INFO("Pipeline Builder: Vertex input set.");
    }

    void VulkanPipelineBuilder::SetInputTopology(VkPrimitiveTopology topology)
    {
        m_inputAssembly.topology = topology;
    }

    void VulkanPipelineBuilder::SetPolygonMode(VkPolygonMode mode)
    {
        m_rasterizer.polygonMode = mode;
    }

    void VulkanPipelineBuilder::SetCullMode(VkCullModeFlags cullMode, VkFrontFace frontFace)
    {
        m_rasterizer.cullMode = cullMode;
        m_rasterizer.frontFace = frontFace;
    }

//    void VulkanPipelineBuilder::SetDepthFormat(VkFormat format)
//    {
//
//    }

    void VulkanPipelineBuilder::EnableDepthClamp(bool enable)
    {
        m_rasterizer.depthClampEnable = enable ? VK_TRUE:VK_FALSE;
    }

    void VulkanPipelineBuilder::EnableDepthBias(bool enable)
    {
        m_rasterizer.depthBiasEnable = enable ? VK_TRUE:VK_FALSE;
    }

    void VulkanPipelineBuilder::EnableBlending(bool enable)
    {
        if(enable)
        {
            m_colorBlendAttachment.blendEnable = VK_TRUE;
            m_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            m_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            m_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            m_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            m_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            m_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
            m_colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        }else
        {
            m_colorBlendAttachment.blendEnable = VK_FALSE;
            m_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            m_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            m_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
            m_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            m_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            m_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
            m_colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        }
    }
}
