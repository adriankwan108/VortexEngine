#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"

namespace vkclass
{
    class VulkanPipelineBuilder
    {
    public:
        VulkanPipelineBuilder();
        
        VkPipeline BuildPipeline(VkDevice device);
        void Clear();
        
        // parameter setters
        void SetShaders();
        
        
    public:
        // fixed function parameters:
        VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
        VkPipelineRasterizationStateCreateInfo _rasterizer;
        VkPipelineMultisampleStateCreateInfo _multisampling;
        VkPipelineDepthStencilStateCreateInfo _depthStencil;
        VkPipelineColorBlendAttachmentState _colorBlendAttachment;
        // viewports and scissors // ?
        
        // shaders
        std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
        VkPipelineLayout _pipelineLayout;
        
        // ?
        VkFormat _colorAttachmentformat;
        VkPipelineRenderingCreateInfo _renderInfo;
    };
}
