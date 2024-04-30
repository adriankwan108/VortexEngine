#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <glm/glm.hpp>

#include "Renderer/OrthographicCamera.hpp"
#include "VulkanBuffer.hpp"

namespace vkclass
{
    class VulkanCamera : public VX::OrthographicCamera
    {
    public:
        VulkanCamera(float left, float right, float bottom, float top): VX::OrthographicCamera(left, right, bottom, top)
        {
            // create uniform buffer
        }
        
        virtual ~VulkanCamera();
        
        // here we update uniform buffer
        virtual void Update() override;
        
        
    private:
        
    };
}
