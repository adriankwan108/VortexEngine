#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "VortexPCH.hpp"

#include "Renderer/GraphicsContext.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "vulkan/vulkan.h"
#include "Core/VulkanDebug.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanInstance.hpp"
#include "Core/VulkanSurface.hpp"
#include "Core/VulkanDevice.hpp"
#include "Core/VulkanCommandManager.hpp"
#include "Core/VulkanSyncManager.hpp"
#include "Core/VulkanDescriptorManager.hpp"
#include "Core/VulkanCamera.hpp"

#include "Core/VulkanBuffer.hpp"

#include "Rendering/VulkanRenderTarget.hpp"

#include "Rendering/VulkanSwapChain.hpp"
#include "Rendering/VulkanSubpass.hpp"
#include "Rendering/VulkanRenderPass.hpp"
#include "Rendering/VulkanShader.hpp"
#include "Rendering/VulkanTexture.hpp"
#include "Rendering/VulkanPipelineBuilder.hpp"
#include "Rendering/VulkanRendererAPI.hpp"
#include "Rendering/VulkanVertexArray.hpp"

#include "Vortex/Geometry/Geometry.hpp"

namespace VX
{
    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(GLFWwindow* window);
        ~VulkanContext();

        virtual void Init() override;
        virtual void DisplayStart() override;
        virtual void DisplayEnd() override;
        virtual void End() override;
        virtual void Resize(unsigned int width, unsigned int height) override;
        
        virtual void SetVSync(bool enable) override;

        const int MAX_FRAMES_IN_FLIGHT = 2;

    private: // TODO: engine configs
        bool m_enableValidation = true;
        bool m_isVSync = true;
        bool m_isFullScreen = false;

    private:
        // extensions required by os
        // std::vector<const char*> m_requiredDeviceExtensions = {};

    private:
        // ref
        GLFWwindow* m_Window = nullptr;
        
        // prop
        vkclass::VulkanInstance m_Instance;
        vkclass::VulkanSurface m_Surface;
        vkclass::VulkanDevice m_Device;
        vkclass::VulkanSwapChain m_SwapChain;
        vkclass::VulkanSyncManager m_SyncManager;
        
        // dynamic resources manager
        vkclass::VulkanCommandManager m_CommandManager;
        vkclass::VulkanRenderTargetManager m_RenderTargetManager;
        
        // vkclass::VulkanDescriptorManager m_DescriptorManager;
        // vkclass::VulkanRenderPassManager m_RenderPassManager;
        

        // std::vector<vkclass::VulkanFrameBuffer*> m_FrameBuffers;
        
    private: // var
        uint32_t m_currentRenderingFrame = 0;
        VkResult m_acquireNextImageResult;
        VkResult m_presentResult;
        bool m_framebufferResized = false;
        
        // window size
        unsigned int m_width;
        unsigned int m_height;
        
    private:
        void resizeHelper();
        
        // void createFrameBuffers();
    };
}
