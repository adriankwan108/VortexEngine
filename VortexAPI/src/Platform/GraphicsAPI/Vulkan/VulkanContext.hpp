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
        GLFWwindow* m_Window = nullptr;
        vkclass::VulkanInstance* m_VulkanInstance = nullptr;
        vkclass::VulkanSurface* m_VulkanSurface = nullptr;
        vkclass::VulkanDevice* m_VulkanDevice = nullptr;
        vkclass::VulkanSwapChain* m_VulkanSwapChain = nullptr;
        
        vkclass::VulkanRenderPass* m_RenderPass = nullptr;
        std::vector<vkclass::VulkanFrameBuffer*> m_VulkanFrameBuffers;
        
        vkclass::VulkanCommandManager* m_VulkanCommandManager = nullptr;
        vkclass::VulkanSyncManager* m_VulkanSyncManager = nullptr;
        
        vkclass::VulkanShader* triangleShader = nullptr;
        
    private:
        /** @brief Create app info and create the encapsulated instance with that info*/
        void initInstance();
        
        /** @brief Create an abstract type of surface for rendering*/
        void initSurface();
        
        /** @brief Create the encapsulated device (physical and logical device) */
        void initDevice();
        
        void initSwapChain();
        
        void initFrameBuffers();
        
        void initCommandManager();
        
        void initSyncManager();
        
        void prepareTriangle();
        void drawTriangle();
    };
}
