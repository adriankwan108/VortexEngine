#pragma once
#include "VortexPCH.hpp"

#include "Renderer/Material.hpp"
#include "VulkanShader.hpp"

namespace vkclass
{
    class VulkanMaterial : public VX::Material
    {
    public:
        VulkanMaterial(VX::Ref<VX::ShaderEffect> shaderEffect);
        virtual ~VulkanMaterial() = default;
        virtual void Bind() override;
        
    private:
        VX::Ref<VulkanShaderEffect> m_shaderEffect;
    
    };
}
