#pragma once
#include "VortexPCH.hpp"
#include "Renderer/GraphicsContext.hpp"

#include "vulkan/vulkan.h"
#include "Base/VulkanDebug.hpp"
#include "Base/VulkanTools.hpp"
#include "Base/VulkanInstance.hpp"
#include "Base/VulkanSurface.hpp"
#include "Base/VulkanDevice.hpp"
#include "Base/VulkanSwapChain.hpp"
#include "Base/VulkanFrameBuffer.hpp"
#include "Base/VulkanSubpass.hpp"
#include "Base/VulkanRenderPass.hpp"
#include "Base/VulkanCommandManager.hpp"
#include "Base/VulkanSyncManager.hpp"
#include "Base/VulkanShader.hpp"
#include "Base/VulkanPipelineBuilder.hpp"

namespace VX
{
    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(GLFWwindow* window);
        ~VulkanContext();

        virtual void Init() override;
        virtual void Display() override;
        virtual void End() override;
        virtual void Resize(unsigned int width, unsigned int height) override;
        
        const int MAX_FRAMES_IN_FLIGHT = 2;

    private: // configs
        bool m_enableValidation = true;
        
    private:
        // extensions required by os
        // std::vector<const char*> m_requiredDeviceExtensions = {};

        
    private: // vulkan objects that are required to be exposed for imgui
        // VkInstance m_Instance = VK_NULL_HANDLE;
//        VkPhysicalDevice m_gpu = VK_NULL_HANDLE;
//        VkDevice m_LogicalDevice = VK_NULL_HANDLE;
        // queue family
        // queue
        // descriptor pool
        // render pass
        // min image count
        // image count
        // msaa samples
        // pipeline cache
        // subpass
        // useDynamicRendering

    private:
        // ref
        GLFWwindow* m_Window = nullptr;
        
        // prop
        vkclass::VulkanInstance m_Instance;
        vkclass::VulkanSurface m_Surface;
        vkclass::VulkanDevice m_Device;
        vkclass::VulkanSyncManager m_SyncManager;
        vkclass::VulkanCommandManager m_CommandManager;
        
        // dynamic life-time, due to recreation by window resize
        vkclass::VulkanSwapChain* m_SwapChain;
        vkclass::VulkanRenderPass* m_RenderPass;
        std::vector<vkclass::VulkanFrameBuffer*> m_FrameBuffers;

        vkclass::VulkanShader* triangleShader = nullptr;
        
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
        
        void createRenderPass();
        void createFrameBuffers();
        
        void prepareTriangle();
        void drawTriangle();
    };
}
