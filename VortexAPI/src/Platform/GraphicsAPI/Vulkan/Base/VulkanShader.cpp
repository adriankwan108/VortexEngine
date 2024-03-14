#include "VulkanShader.hpp"

namespace vkclass
{
    VkDevice vkclass::VulkanShader::m_device = VK_NULL_HANDLE;

    VulkanShader::VulkanShader(const std::string& filepath):m_FilePath(filepath)
    {
        // read file
        std::vector<char> shaderCode = VX::Utils::readFile(m_FilePath);
        if (shaderCode.empty())
        {
            VX_CORE_WARN("Shader files not exist");
            return;
        }
        
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
        VK_CHECK_RESULT(vkCreateShaderModule(m_device, &createInfo, nullptr, &m_shaderModule));
        m_isValid = true;
    }

    VulkanShader::~VulkanShader()
    {
        if (m_isValid)
        {
            vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
            VX_CORE_INFO("Shader destroyed");
        }
    }

    void VulkanShader::Init(VkDevice device)
    {
        m_device = device;
    }

    void VulkanShader::Bind()
    {
        // vkcmdbindpipeline
    }

    void VulkanShader::UnBind()
    {
        
    }


}
