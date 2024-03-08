#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"

#include "Vortex/Utils.hpp"

namespace vkclass
{
    class VulkanShader
    {
    public:
        VulkanShader(const std::string& filepath);
        ~VulkanShader();
        
        void Bind();
        void UnBind();
        
    public:
        const VkShaderModule& ShaderModule = m_shaderModule;
        
//         static VkDevice m_device;
//         static void Init(VkDevice device);
        
    private:
        uint32_t m_RendererID;
        std::string m_Name;
        std::string m_FilePath;
        
        VkShaderModule m_shaderModule;
    };
}
