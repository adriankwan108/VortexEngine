#include "VulkanPipelineBuilder.hpp"

namespace vkclass
{
    VulkanPipelineBuilder::VulkanPipelineBuilder()
    {
        Clear();
    }

    VkPipeline VulkanPipelineBuilder::BuildPipeline(VkDevice device)
    {
        VkPipeline newPipeline;

        
        return newPipeline;
    }

    void VulkanPipelineBuilder::Clear()
    {
        // reset all parameters to zero
        _inputAssembly = { .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
        _rasterizer = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
        _multisampling = { .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
        _depthStencil = { .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
        _colorBlendAttachment = {};
        
        _shaderStages.clear();
        _pipelineLayout = {};
       
        _renderInfo = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
    }
}
