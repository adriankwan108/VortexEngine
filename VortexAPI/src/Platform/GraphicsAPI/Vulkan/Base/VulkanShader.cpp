#include "VulkanShader.hpp"

namespace vkclass
{
    VkDevice vkclass::VulkanShader::m_device = VK_NULL_HANDLE;

    VulkanShader::VulkanShader(const std::string& filepath):m_FilePath(filepath)
    {
        // read file
        std::vector<char> shaderCode = VX::Utils::readFile(m_FilePath);
        
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
        VK_CHECK_RESULT(vkCreateShaderModule(m_device, &createInfo, nullptr, &m_shaderModule));
    }

    VulkanShader::~VulkanShader()
    {
        vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
        VX_CORE_INFO("Destroy shader");
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
