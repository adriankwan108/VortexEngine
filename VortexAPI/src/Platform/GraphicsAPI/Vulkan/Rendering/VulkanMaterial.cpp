#include "VulkanMaterial.hpp"

namespace vkclass
{
    VulkanMaterial::VulkanMaterial(VX::Ref<VX::ShaderEffect> shaderEffect)
    {
        m_shaderEffect = std::static_pointer_cast<VulkanShaderEffect>(shaderEffect);
    }

    void VulkanMaterial::Bind()
    {
        m_shaderEffect->Bind();
        
        // descriptor sets binding (pipelinelayout, firstSet, set count, *descriptor set)
    }
}
