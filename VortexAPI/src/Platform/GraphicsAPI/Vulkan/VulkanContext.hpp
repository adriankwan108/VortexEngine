#pragma once
#include "VortexPCH.hpp"
#include "Renderer/GraphicsContext.hpp"

#include "vulkan/vulkan.h"
#include "Base/VulkanInstance.hpp"
#include "Base/VulkanDebug.hpp"
#include "Base/VulkanDevice.hpp"
#include "Base/VulkanTools.hpp"

namespace VX
{
    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(GLFWwindow* window);
        ~VulkanContext();

        virtual void Init() override;
        virtual void Display() override;

    private: // configs
        bool m_enableValidation = true;
        
    private:
        // extensions required by os
        // std::vector<const char*> m_requiredDeviceExtensions = {};

        
    private: // vulkan objects that are required to be exposed for imgui
        // VkInstance m_Instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_gpu = VK_NULL_HANDLE;
        VkDevice m_LogicalDevice = VK_NULL_HANDLE;
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
         vkclass::VulkanInstance* m_VulkanInstance = nullptr;
         vkclass::VulkanDevice* m_VulkanDevice = nullptr;
        
    private:
        /** @brief Create app info and create the encapsulated instance with that info*/
        void initInstance(bool enableValidation);
        
        /** @brief Create the encapsulated device (physical and logical device) */
        void initDevice();
    };
}
