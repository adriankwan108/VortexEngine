#include "VulkanShader.hpp"
#include "Core/VulkanCamera.hpp"

// testing
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spirv_cross/spirv_glsl.hpp>

namespace vkclass
{
    //VkDevice vkclass::VulkanShader::m_device = VK_NULL_HANDLE;
    //vkclass::VulkanCommandManager* vkclass::VulkanShader::m_commandBufferManager = nullptr;
    //VkRenderPass vkclass::VulkanShader::s_RenderPass = VK_NULL_HANDLE;

    //VulkanShader::~VulkanShader()
    //{
    //    if (m_isValid)
    //    {
    //        if(m_pipeline != VK_NULL_HANDLE)
    //        {
    //            vkDestroyPipeline(m_device, m_pipeline, nullptr);
    //        }
    //        
    //        if(m_pipelineLayout != VK_NULL_HANDLE)
    //        {
    //            vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
    //        }
    //        VX_CORE_INFO("VulkanShader: Pipeline & layout destroyed");
    //    }
    //}

    //void VulkanShader::Init(VkDevice device, vkclass::VulkanCommandManager* commandBufferManager, VkRenderPass renderPass)
    //{
    //    m_device = device;
    //    m_commandBufferManager = commandBufferManager;
    //    s_RenderPass = renderPass;
    //}

    //void VulkanShader::Bind()
    //{
    //    // bind descriptor per material / object
    //    // m_commandBufferManager->BindDescriptor(m_pipelineLayout, 0, &m_textureDescriptor->GetCurrentSet());

    //    /*for (auto& [range, pvalue] : m_pushConstants)
    //    {
    //        m_commandBufferManager->PushConstant(m_pipelineLayout, range.stageFlags, range.size, pvalue);
    //    }*/
    //    m_commandBufferManager->BindPipeline(m_pipeline);
    //}

    //void VulkanShader::UnBind() const
    //{
    //    
    //}

    //void VulkanShader::SetVertexLayout(VX::VertexShaderLayout layout)
    //{
    //    m_vertexLayout.SetLayout(layout);
    //}

    //
    //void VulkanShader::SetGlobalLayout(int binding, VX::UniformShaderLayout layout)
    //{
    //    // TODO: add name to UniformShaderLayout, then search global desciprot library

    //    m_descriptorSetLayouts.push_back( GlobalDescriptor::GetDescriptor()->layout);
    //}

    //void VulkanShader::SetPassLayout(int binding, VX::UniformShaderLayout layout)
    //{
    //     
    //}

    //void VulkanShader::SetTexture(VX::Ref<VX::Texture2D> texture)
    //{
    //    m_texture = std::static_pointer_cast<VulkanTexture2D>(texture);
    //    
    //    m_textureDescriptor = m_texture->GetDescriptor();
    //    m_descriptorSetLayouts.push_back(m_textureDescriptor->layout);
    //}

    //void VulkanShader::SetPushConstant(VkShaderStageFlags stage, uint32_t size, void* pValue)
    //{
    //    m_pushConstants.push_back( 
    //        std::make_pair(vkclass::initializers::pushConstantRange(stage, size, 0), pValue)
    //    );
    //}

    //void VulkanShader::SetObjectLayout(int binding, VX::UniformShaderLayout layout)
    //{
    //    
    //}

    //void VulkanShader::Prepare()
    //{
    //    VX_CORE_TRACE("VulkanShader: Preparing pipeline...");
    //    VulkanPipelineBuilder builder;
    //    builder.SetShaders(m_vertModule, m_fragModule);
    //    builder.SetVertexInput(m_vertexLayout.GetBinding(), m_vertexLayout.GetAttributes());
    //    builder.SetCullMode(VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
    //    
    //    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    //    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    //    VX_CORE_TRACE("VulkanShader: descriptor layout count: {0}", m_descriptorSetLayouts.size());
    //    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_descriptorSetLayouts.size());
    //    pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();

    //    std::vector<VkPushConstantRange> vkPushConstantRanges;
    //    std::transform(m_pushConstants.begin(), m_pushConstants.end(), std::back_inserter(vkPushConstantRanges),
    //        [](const std::pair<VkPushConstantRange, void*>& pair) {
    //            return pair.first;
    //        });
    //    pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(m_pushConstants.size());
    //    pipelineLayoutInfo.pPushConstantRanges = vkPushConstantRanges.data();
    //    if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    //    {
    //        m_isValid = false;
    //        VX_CORE_WARN("VulkanShader: Failed to create pipeline layout!");
    //        return;
    //    }
    //    
    //    // build operation
    //    m_pipeline = builder.BuildPipeline(m_pipelineLayout, s_RenderPass);
    //    
    //    // destroy unused resources
    //    vkDestroyShaderModule(m_device, m_vertModule, nullptr);
    //    vkDestroyShaderModule(m_device, m_fragModule, nullptr);
    //    
    //    VX_CORE_TRACE("Vulkan Shader: Pipeline set");
    //}

    VkDevice vkclass::VulkanShader::s_device = VK_NULL_HANDLE;

    VulkanShader::VulkanShader(const std::string& name, const std::string& filePath, VX::ShaderStage stage)
    {
        m_name = name;
        m_filePath = filePath;
        m_stage = stage;
        m_layoutGroups["Input"] = std::vector<VX::ShaderBlock>();

        std::vector<uint32_t> shaderCode = VX::Utils::readFile(m_filePath);
            
        if (shaderCode.empty())
        {
            VX_CORE_WARN("VulkanShader: {0} read failed. Path: {1}", m_name, m_filePath);
            return;
        }

        VkShaderModuleCreateInfo moduleCI{};
        moduleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCI.codeSize = shaderCode.size() * sizeof(uint32_t);
        moduleCI.pCode = shaderCode.data();
        VK_CHECK_RESULT(vkCreateShaderModule(s_device, &moduleCI, nullptr, &m_module));

        reflect(shaderCode);
    }

    VulkanShader::~VulkanShader()
    {
        if (m_module != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(s_device, m_module, nullptr);
        }
    }

    void VulkanShader::reflect(const std::vector<uint32_t>& data)
    {
        VX_CORE_TRACE("VulkanShader: Reflecting...");
        spirv_cross::CompilerGLSL glsl(data);
        spirv_cross::ShaderResources resources = glsl.get_shader_resources();

        for (auto& resource: resources.stage_inputs)
        {
            unsigned location = glsl.get_decoration(resource.id, spv::DecorationLocation);
            const spirv_cross::SPIRType& type = glsl.get_type(resource.type_id);
            
            m_layoutGroups["Input"].push_back(
                VX::ShaderBlock(location, -1, -1, {
                    VX::ShaderElement(VX::SpirTypeToShaderDataType(type), resource.name.c_str())
                })
            );
        }

        for (auto& resource : resources.uniform_buffers)
        {
            /*unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);

            VX_CORE_TRACE("Reflect: UniformBuffer {0} at set = {1}, binding = {2}", resource.name.c_str(), set, binding);*/
            VX_CORE_TRACE("Reflect: {0}", resource.name);
            
            auto& type = glsl.get_type(resource.base_type_id);
            
            unsigned int member_count = static_cast<unsigned int>(type.member_types.size());
            for (unsigned i = 0; i < member_count; i++)
            {
                auto& member_type = glsl.get_type(type.member_types[i]);
                size_t memberSize = glsl.get_declared_struct_member_size(type, i);

                // Get member offset within this struct.
                size_t offset = glsl.type_struct_member_offset(type, i);

                if (!member_type.array.empty())
                {
                    VX_CORE_TRACE("array found");
                    // Get array stride, e.g. float4 foo[]; Will have array stride of 16 bytes.
                    size_t array_stride = glsl.type_struct_member_array_stride(type, i);
                }

                
                //const std::string& memberName = glsl.get_member_name(type.self, i);
                
                //VX_CORE_TRACE("Reflect: UniformBuffer: {0}, {1}, {2}", memberName, memberSize, offset);
            }
        }

        for (auto& resource : resources.sampled_images)
        {
            unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);

            VX_CORE_TRACE("Reflect: Sampler {0} at set = {1}, binding = {2}", resource.name.c_str(), set, binding);
        }

        for (auto& resource : resources.push_constant_buffers)
        {
            unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);

            VX_CORE_TRACE("Reflect: PushConstant {0} at set = {1}, binding = {2}", resource.name.c_str(), set, binding);
        }
    }

    void VulkanShader::Init(VkDevice device)
    {
        s_device = device;
    }

    VulkanShaderPass::VulkanShaderPass()
    {
        
    }

    void VulkanShaderPass::Prepare()
    {
        for(auto& shader : m_shaders)
        {
            // determine shader stage
            
            // vertex: prepare vertex attribute from layout[input]
            
            // prepare descriptor sets layouts
        }
        
        // prepare pipeline layouts from all shader modules
    }
}
