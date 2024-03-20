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
        
        static void Init(VkDevice device);
        
        VkPipeline BuildPipeline(VkPipelineLayout layout, VkRenderPass renderPass);
        void Clear();
        
        // parameter setters
        void SetShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
        void SetVertexInput(VkVertexInputBindingDescription* binding, VkVertexInputAttributeDescription* attribute); // TODO: this should be set automatically after setting input layout in creating shader
        void SetInputTopology(VkPrimitiveTopology topology);
        void SetPolygonMode(VkPolygonMode mode);
        void SetCullMode(VkCullModeFlags cullMode, VkFrontFace frontFace);
//        void SetDepthFormat(VkFormat format);
        
        // options, disable by default (set false by initializer) unless called
        void EnableDepthClamp(bool enable = true); // rasterizer stuff
        void EnableDepthBias(bool enable = true); // rasterizer stuff
        void EnableMultiSampling(bool enable = true);
//        void EnableDepthStencil(bool enable = true);
        void EnableBlending(bool enable = true);
        void EnableDepthTest(bool enable = true);
        
    public:
        // default
        const VkPrimitiveTopology DefaultTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        const VkPolygonMode DefaultPolygonMode = VK_POLYGON_MODE_FILL; // other than fill require gpu feature
        const VkCullModeFlags DefaultCullMode = VK_CULL_MODE_BACK_BIT;
        const VkFrontFace DefaultFrontFace = VK_FRONT_FACE_CLOCKWISE;
        
    private:
        static VkDevice m_device;
        
        // shaders
        std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
        
        // fixed function parameters:
        VkPipelineVertexInputStateCreateInfo m_vertexInputInfo{};
        VkPipelineInputAssemblyStateCreateInfo m_inputAssembly{};
        VkPipelineViewportStateCreateInfo m_viewportState{}; // this would be handled by dynamic,
        VkPipelineRasterizationStateCreateInfo m_rasterizer{};
        VkPipelineMultisampleStateCreateInfo m_multisampling{};
//        VkPipelineDepthStencilStateCreateInfo m_depthStencil{};
        
        VkPipelineColorBlendAttachmentState m_colorBlendAttachment{}; // configs per attached framebuffer
        VkPipelineColorBlendStateCreateInfo m_colorBlending{}; // global color blending setting
        
        // dynamic for viewports, scissor
        std::vector<VkDynamicState> m_dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
    };
}
