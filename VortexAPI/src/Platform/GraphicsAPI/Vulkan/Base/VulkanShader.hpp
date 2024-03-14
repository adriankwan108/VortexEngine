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
        static void Init(VkDevice device);
        
        bool& Valid = m_isValid;
    private:
        static VkDevice m_device;
        
        uint32_t m_RendererID = 0;
        std::string m_Name;
        std::string m_FilePath;
        
        VkShaderModule m_shaderModule = VK_NULL_HANDLE;
        bool m_isValid = false;
    };
}
