#pragma once
#include "VortexPCH.hpp"
#include "Vortex/Utils.hpp"

#include "vulkan/vulkan.h"
#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanPipelineBuilder.hpp"
#include "VulkanCommandManager.hpp"

#include "VulkanGeometry.hpp"

namespace vkclass
{
    class VulkanShader
    {
    public:
        VulkanShader(const std::string& vertFilePath, const std::string fragFilePath);
        ~VulkanShader();
        
        static void Init(VkDevice device, vkclass::VulkanCommandManager* commandBufferManager);
        
        void Bind();
        void UnBind();
        void CreatePipeline(VkRenderPass renderPass);
        
    public:
        const VkShaderModule& VertModule = m_vertModule;
        const VkShaderModule& FragModule = m_fragModule;
        const VkPipeline& Pipeline = m_pipeline;
        
        bool& Valid = m_isValid;
        
    private:
        static VkDevice m_device;
        static vkclass::VulkanCommandManager* m_commandBufferManager;
        
        // props
        uint32_t m_RendererID = 0;
        std::string m_Name;
        
        std::string m_vertFilePath;
        std::string m_fragFilePath;
        
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
        
        // private var
        VkShaderModule m_vertModule = VK_NULL_HANDLE;
        VkShaderModule m_fragModule = VK_NULL_HANDLE;
        bool m_isValid = false;
        vkclass::VulkanPipelineBuilder m_pipelineBuilder;
    };
}
