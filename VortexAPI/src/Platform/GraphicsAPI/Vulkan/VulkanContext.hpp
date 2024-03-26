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

#include "Core/VulkanBuffer.hpp"

#include "Rendering/VulkanFrameBuffer.hpp"
#include "Rendering/VulkanSwapChain.hpp"
#include "Rendering/VulkanSubpass.hpp"
#include "Rendering/VulkanRenderPass.hpp"
#include "Rendering/VulkanShader.hpp"
#include "Rendering/VulkanPipelineBuilder.hpp"

#include "Geometry/VulkanGeometry.hpp"

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
        
        // helpers
        vkclass::VulkanPipelineBuilder m_pipelineBuilder;

        // a triangle resources
        vkclass::VulkanShader* triangleShader = nullptr;
        vkclass::VulkanVertexBuffer* vertexBuffer = nullptr;
        vkclass::VulkanIndexBuffer* indexBuffer = nullptr;
        
    private: // var
        uint32_t m_currentRenderingFrame = 0;
        VkResult m_acquireNextImageResult;
        VkResult m_presentResult;
        bool m_framebufferResized = false;
        
        // window size
        unsigned int m_width;
        unsigned int m_height;
        
        // simulate data
        std::vector<Geometry::Vertex> triangleVertices =
        {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };
        
        std::vector<uint16_t> triangleIndices = {
            0, 1, 2, 2, 3, 0
        };
        
    private:
        void resizeHelper();
        
        void createRenderPass();
        void createFrameBuffers();
        
        void prepareTriangle();
        void drawTriangle();
    };
}
