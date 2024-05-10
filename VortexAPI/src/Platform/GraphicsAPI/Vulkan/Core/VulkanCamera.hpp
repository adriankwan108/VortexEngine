#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <glm/glm.hpp>

#include "Renderer/OrthographicCamera.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanDescriptorManager.hpp"

#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"
#include "Vortex/Geometry/Geometry.hpp"

namespace vkclass
{
    class VulkanCamera : public VX::OrthographicCamera
    {
    public:
        VulkanCamera(float left, float right, float bottom, float top): 
            VX::OrthographicCamera(left, right, bottom, top)
            , m_uniformBuffer(sizeof(Geometry::Uniform_VP))
        {
            setDescriptor();
            Update();
        }
        
        virtual ~VulkanCamera();
        
        // here we update uniform buffer
        virtual void Update() override;
        
        static void Init(VulkanDevice* device);
    public:
        std::shared_ptr<VulkanDescriptor> GetDescriptor() const { return m_descriptor; }
        
    private:
        static VulkanDevice* m_device;
        std::shared_ptr<VulkanDescriptor> m_descriptor;
        VulkanUniformBuffer m_uniformBuffer;
        
        Geometry::Uniform_VP camera_vp{};
        void setDescriptor();
    };
}
