#include "VulkanShader.hpp"
#include "Core/VulkanCamera.hpp"

// testing
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vkclass
{
    VkDevice vkclass::VulkanShader::m_device = VK_NULL_HANDLE;
    vkclass::VulkanCommandManager* vkclass::VulkanShader::m_commandBufferManager = nullptr;
    VkRenderPass vkclass::VulkanShader::s_RenderPass = VK_NULL_HANDLE;

    VulkanShader::VulkanShader(const std::string& name, const std::string& vertFilePath, const std::string fragFilePath)
        : m_Name(name), m_vertFilePath(vertFilePath),m_fragFilePath(fragFilePath),
            m_uniformBuffer(sizeof(Geometry::Uniform_VP))
    {
        // read file
        std::vector<char> vertShaderCode = VX::Utils::readFile(m_vertFilePath);
        std::vector<char> fragShaderCode = VX::Utils::readFile(m_fragFilePath);
        
        if (vertShaderCode.empty() || fragShaderCode.empty())
        {
            VX_CORE_WARN("Vulkan Shader: Read failed. Shader files require both non-empty .vert & .frag");
            return;
        }
        
        VkShaderModuleCreateInfo vertModuleCI{};
        vertModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertModuleCI.codeSize = vertShaderCode.size();
        vertModuleCI.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());
        if(vkCreateShaderModule(m_device, &vertModuleCI, nullptr, &m_vertModule) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_INFO("Vulkan Shader: Failed to create vert shader module");
            return;
        }
        
        VkShaderModuleCreateInfo fragModuleCI{};
        fragModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragModuleCI.codeSize = fragShaderCode.size();
        fragModuleCI.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());
        if(vkCreateShaderModule(m_device, &fragModuleCI, nullptr, &m_fragModule) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_INFO("Vulkan Shader: Failed to create frag shader module");
            return;
        }
        
        m_isValid = true;
        // TODO: if not valid, use default shader (guaranteed)
        
        
    }

    VulkanShader::~VulkanShader()
    {
        vkDestroyDescriptorPool(m_device, test_pool, nullptr);
        vkDestroyDescriptorSetLayout(m_device, test_descriptorSetLayout, nullptr);
        if (m_isValid)
        {
            if(m_pipeline != VK_NULL_HANDLE)
            {
                vkDestroyPipeline(m_device, m_pipeline, nullptr);
            }
            
            if(m_pipelineLayout != VK_NULL_HANDLE)
            {
                vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
            }
            VX_CORE_INFO("VulkanShader: Pipeline & layout destroyed");
        }
    }

    void VulkanShader::Init(VkDevice device, vkclass::VulkanCommandManager* commandBufferManager, VkRenderPass renderPass)
    {
        m_device = device;
        m_commandBufferManager = commandBufferManager;
        s_RenderPass = renderPass;
    }

    void VulkanShader::Bind()
    {
        test_vp.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        test_vp.proj = glm::perspective(glm::radians(45.0f), 1920 / (float) 1080, 0.1f, 10.0f);
        m_uniformBuffer.Update(&test_vp, sizeof(test_vp));
        
        // bind
        m_commandBufferManager->BindDescriptor(m_pipelineLayout, &test_descriptorSets[VulkanBuffer::GetCurrentFrame()]);
        m_commandBufferManager->BindPipeline(m_pipeline);
    }

    void VulkanShader::UnBind() const
    {
        
    }

    void VulkanShader::SetVertexLayout(VX::VertexShaderLayout layout)
    {
        m_vertexLayout.SetLayout(layout);
    }

    
    void VulkanShader::SetGlobalLayout(int binding, VX::UniformShaderLayout layout)
    {
        // TODO: add name to UniformShaderLayout, then search global desciprot library
//        VX_CORE_INFO("VulkanShader: setting global layout...");
//        m_descriptorSetLayouts.push_back( GlobalDescriptor::GetDescriptor()->layout);
//        VX_CORE_INFO("VulkanShader: global layout set.");
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &test_descriptorSetLayout) != VK_SUCCESS)
        {
            VX_CORE_ERROR("VulkanShader: Failed to create descriptor set layout.");
            throw std::runtime_error("failed to create descriptor set layout!");
        }
        
    }

    void VulkanShader::SetPassLayout(int binding, VX::UniformShaderLayout layout)
    {
         
    }
    void VulkanShader::SetMaterialLayout(int binding, VX::UniformShaderLayout layout)
    {
        
    }

    void VulkanShader::SetObjectLayout(int binding, VX::UniformShaderLayout layout)
    {
        
    }

    void VulkanShader::Prepare()
    {
        VX_CORE_TRACE("Vulkan Shader: Preparing pipeline...");
        VulkanPipelineBuilder builder;
        builder.SetShaders(m_vertModule, m_fragModule);
        builder.SetVertexInput(m_vertexLayout.GetBinding(), m_vertexLayout.GetAttributes());
        // builder.SetCullMode(VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_descriptorSetLayouts.size());
//        pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &test_descriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
        if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            m_isValid = false;
            VX_CORE_WARN("VulkanShader: Failed to create pipeline layout!");
            return;;
        }
        
        // build operation
        m_pipeline = builder.BuildPipeline(m_pipelineLayout, s_RenderPass);
        
        // destroy unused resources
        vkDestroyShaderModule(m_device, m_vertModule, nullptr);
        vkDestroyShaderModule(m_device, m_fragModule, nullptr);
        
        VX_CORE_TRACE("Vulkan Shader: Pipeline set");
        
        // testing
        // pools
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(1000);
        
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32_t>(1000);
        
        if (vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &test_pool) != VK_SUCCESS)
        {
            VX_CORE_ERROR("VulkanShader: Failed to create descriptorl pool.");
            throw std::runtime_error("failed to create descriptor pool!");
        }
        
        // allocate
        std::vector<VkDescriptorSetLayout> layouts(2, test_descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = test_pool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(2);
        allocInfo.pSetLayouts = layouts.data();

        test_descriptorSets.resize(2);
        if (vkAllocateDescriptorSets(m_device, &allocInfo, test_descriptorSets.data()) != VK_SUCCESS)
        {
            VX_CORE_ERROR("VulkanShader: Failed to allocate descriptor sets.");
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
        
//        test_descriptorSets.resize(2);
//
//        DescriptorManager::Allocate(test_descriptorSetLayout, test_descriptorSets);
        
        // write
        for (size_t i = 0; i < 2; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_uniformBuffer.m_UniformBuffers[i].Buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(Geometry::Uniform_VP);
            
            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = test_descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            
            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr; // Optional
            descriptorWrite.pTexelBufferView = nullptr; // Optional
            
            vkUpdateDescriptorSets(m_device, 1, &descriptorWrite, 0, nullptr);
            
            
        }
    }

}
