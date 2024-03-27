#pragma once
#include "VortexPCH.hpp"
#include "Vortex/Utils.hpp"

#include "vulkan/vulkan.h"

#include "Renderer/Shader.hpp"
#include "Core/VulkanInitializer.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanCommandManager.hpp"
#include "VulkanPipelineBuilder.hpp"

#include "Geometry/VulkanGeometry.hpp"

namespace vkclass
{
    class VulkanShader : public VX::Shader
    {
    public:
        VulkanShader(const std::string& name, const std::string& vertFilePath, const std::string fragFilePath);
        ~VulkanShader();
        
        virtual void Bind() const override;
        virtual void UnBind() const override;
        
        virtual const std::string& GetName() const override { return m_Name; }
        
    public:
        const VkShaderModule& VertModule = m_vertModule;
        const VkShaderModule& FragModule = m_fragModule;
        const VkPipeline& Pipeline = m_pipeline;
        const VkPipelineLayout& PipelineLayout = m_pipelineLayout;
    
        bool& Valid = m_isValid;
        
        void SetPipeline(VkPipeline pipeline);
        
    public:
        static void Init(VkDevice device, vkclass::VulkanCommandManager* commandBufferManager);
        
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
    };
}
