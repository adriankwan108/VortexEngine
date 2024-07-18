#pragma once
#include "VortexPCH.hpp"

#include "Renderer/Material.hpp"
#include "VulkanShader.hpp"

namespace vkclass
{
    class VulkanMaterial : public VX::Material
    {
    public:
        virtual ~VulkanMaterial() = default;
        virtual void Bind() override;
        
    private:
        
    
    }
}
