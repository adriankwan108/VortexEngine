#include "VortexPCH.hpp"
#include "VulkanTexture.hpp"

#include "stb_image.hpp"
#include "Vortex/Utils.hpp"

namespace vkclass
{
    VulkanDevice* VulkanTexture2D::s_device = nullptr;
    VulkanCommandManager* VulkanTexture2D::s_cmdManager = nullptr;

    VulkanTexture2D::VulkanTexture2D()
    {
    }

    VulkanTexture2D::~VulkanTexture2D()
    {
        if (m_sampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(s_device->LogicalDevice, m_sampler, nullptr);
        }

        if (m_imageView != VK_NULL_HANDLE)
        {
            VX_CORE_TRACE("destroying image view");
            vkDestroyImageView(s_device->LogicalDevice, m_imageView, nullptr);
        }

        if (m_image != VK_NULL_HANDLE)
        {
            vkDestroyImage(s_device->LogicalDevice, m_image, nullptr);
        }

        if (m_imageMemory != VK_NULL_HANDLE)
        {
            vkFreeMemory(s_device->LogicalDevice, m_imageMemory, nullptr);
        }
    }

    void VulkanTexture2D::LoadFromFile(std::string path)
    {
        m_path = path;

        // loading by staging buffer
        VX_CORE_INFO("Texture: Loading from {0} ...", m_path);
        int texWidth, texHeight, texChannels;
        stbi_uc* pixelsData = stbi_load( VX::Utils::AbsolutePath(m_path).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        VX_CORE_ASSERT( pixelsData,"VulkanTexture2D: Failed to load image!");
        m_Width  = static_cast<uint32_t>(texWidth);
        m_Height = static_cast<uint32_t>(texHeight);

        vkclass::VulkanBuffer stagingBuffer = vkclass::VulkanBuffer(pixelsData, imageSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        createImage(&stagingBuffer);
        createImageView(&m_image, &m_imageView);
        createSampler(&m_sampler);

        stbi_image_free(pixelsData);

        // retreive infos
        m_imgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        m_imgInfo.imageView = m_imageView;
        m_imgInfo.sampler = m_sampler;

        // desriptor
        /*m_samplerDescriptor = DescriptorManager::CreateDescriptor();
        m_samplerDescriptor->SetStage(VK_SHADER_STAGE_FRAGMENT_BIT);
        m_samplerDescriptor->AddBinding(0, &m_imgInfo);
        m_samplerDescriptor->Build();
        m_samplerDescriptor->Allocate();
        m_samplerDescriptor->Update();*/
    }

    void VulkanTexture2D::LoadFromData(unsigned char* data, int width, int height)
    {
        // loading by staging buffer
        VX_CORE_INFO("Texture: Loading from data ...");
        m_Width  = static_cast<uint32_t>(width);
        m_Height = static_cast<uint32_t>(height);
        VkDeviceSize uploadSize = m_Width * m_Height * 4 * sizeof(char);

        vkclass::VulkanBuffer stagingBuffer = vkclass::VulkanBuffer(data, uploadSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

        createImage(&stagingBuffer);
        createImageView(&m_image, &m_imageView);
        createSampler(&m_sampler);

        // retreive infos
        m_imgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        m_imgInfo.imageView = m_imageView;
        m_imgInfo.sampler = m_sampler;

        // desriptor
        /*m_samplerDescriptor = DescriptorManager::CreateDescriptor();
        m_samplerDescriptor->SetStage(VK_SHADER_STAGE_FRAGMENT_BIT);
        m_samplerDescriptor->AddBinding(0, &m_imgInfo);
        m_samplerDescriptor->Build();
        m_samplerDescriptor->Allocate();
        m_samplerDescriptor->Update();*/
    }

    void VulkanTexture2D::Init(VulkanDevice* device, VulkanCommandManager* cmdManager)
    {
        s_device = device;
        s_cmdManager = cmdManager;
    }

    void VulkanTexture2D::Bind(uint32_t slot)
    {

    }

    void VulkanTexture2D::createImage(vkclass::VulkanBuffer* stagingBuffer)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = m_Width;
        imageInfo.extent.height = m_Height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        
        imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // only for graphics(transfer inclusive)
        
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0; // Optional
        
        if (vkCreateImage(s_device->LogicalDevice, &imageInfo, nullptr, &m_image) != VK_SUCCESS)
        {
            VX_CORE_ERROR("VulkanTexture: Failed to create image!");
            throw std::runtime_error("failed to create image!");
        }
        
        VkDeviceSize allocSize;
        uint32_t memType;
        s_device->GetMemoryInfo(m_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, allocSize, memType);
        
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = allocSize;
        allocInfo.memoryTypeIndex = memType;

        if (vkAllocateMemory(s_device->LogicalDevice, &allocInfo, nullptr, &m_imageMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }
        
        vkBindImageMemory(s_device->LogicalDevice, m_image, m_imageMemory, 0);

        s_cmdManager->TransitImageLayout(m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        s_cmdManager->CopyBufferToImage(stagingBuffer->Buffer, m_image, m_Width, m_Height);
        s_cmdManager->TransitImageLayout(m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    
        VX_CORE_TRACE("VulkanTexture: Create Image");
    }

    void VulkanTexture2D::createImageView(VkImage* image, VkImageView* imageView)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = *image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VK_CHECK_RESULT(vkCreateImageView(s_device->LogicalDevice, &viewInfo, nullptr, imageView));

        if (imageView == VK_NULL_HANDLE)
        {
            VX_CORE_TRACE("image view null wtf");
        }

        VX_CORE_TRACE("VulkanTexture: Create Image View");
    }

    void VulkanTexture2D::createSampler(VkSampler* sampler)
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;

        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = s_device->DeviceProps.limits.maxSamplerAnisotropy;
        /*samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;*/

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        VK_CHECK_RESULT(vkCreateSampler(s_device->LogicalDevice, &samplerInfo, nullptr, sampler));

        VX_CORE_TRACE("VulkanTexture: Create sampler");
    }
}
