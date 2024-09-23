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
        m_layoutGroups["Input"]     = std::vector<VX::ShaderBlock>();
        m_layoutGroups["UniformBuffers"] = std::vector<VX::ShaderBlock>();

        read();
        reflect();
    }

    VulkanShader::~VulkanShader()
    {
        ClearModule();
    }

    void VulkanShader::Init(VkDevice device)
    {
        s_device = device;
    }

    bool VulkanShader::read()
    {
        m_shaderCode = VX::Utils::readFile(m_filePath);
            
        if (m_shaderCode.empty())
        {
            VX_CORE_WARN("VulkanShader: {0} read failed. Path: {1}", m_name, m_filePath);
            return false;
        }
        
        m_isRead = true;
        return true;
    }

    void VulkanShader::reflect()
    {
        if(!m_isRead)
        {
            VX_CORE_WARN("VulkanShader::reflect: shader is not yet read.");
            return;
        }
        
        spirv_cross::CompilerGLSL glsl(m_shaderCode);
        spirv_cross::ShaderResources resources = glsl.get_shader_resources();

        VX_CORE_INFO("VulkanShader: Reflecting...");
        for (auto& resource: resources.stage_inputs)
        {
            unsigned location = glsl.get_decoration(resource.id, spv::DecorationLocation);
            const spirv_cross::SPIRType& type = glsl.get_type(resource.type_id);
            
            m_layoutGroups["Input"].push_back(
                VX::ShaderBlock(location, 0, 0, {
                    VX::ShaderElement(VX::SpirTypeToShaderDataType(type), resource.name.c_str())
                })
            );
        }

        for (auto& resource : resources.uniform_buffers)
        {
            unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
            const spirv_cross::SPIRType& type = glsl.get_type(resource.base_type_id);
            
            unsigned int member_count = static_cast<unsigned int>(type.member_types.size());
            std::vector<VX::ShaderElement> elements;
            for (unsigned i = 0; i < member_count; i++)
            {
                auto& member_type = glsl.get_type(type.member_types[i]);
                size_t memberSize = glsl.get_declared_struct_member_size(type, i);
                const std::string& memberName = glsl.get_member_name(type.self, i);

                // Get member offset within this struct.
                size_t offset = glsl.type_struct_member_offset(type, i);

                if (!member_type.array.empty())
                {
                    // Get array stride, e.g. float4 foo[]; Will have array stride of 16 bytes.
                    size_t array_stride = glsl.type_struct_member_array_stride(type, i);
                    VX_CORE_TRACE("VulkanShader::Reflect: Array is not yet supported in uniform buffers.");
                }

                elements.push_back(VX::ShaderElement(VX::SpirTypeToShaderDataType(member_type), memberName));
            }
            m_layoutGroups["UniformBuffers"].push_back(
                VX::ShaderBlock(0, set, binding, elements)
            );
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
        
        m_isReflected = true;
    }

    void VulkanShader::loadModule()
    {
        if(m_isRead == false)
        {
            if(read() == false)
            {
                VX_CORE_WARN("VulkanShader::loadMoudle: read failed.");
                return;
            }
        }
        if(m_isReflected == false) reflect();
        
        VkShaderModuleCreateInfo moduleCI{};
        moduleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCI.codeSize = m_shaderCode.size() * sizeof(uint32_t);
        moduleCI.pCode = m_shaderCode.data();
        VK_CHECK_RESULT(vkCreateShaderModule(s_device, &moduleCI, nullptr, &m_module));
    }

    const VkShaderModule& VulkanShader::GetModule()
    {
        if(!isModuleLoaded)
        {
            loadModule();
            isModuleLoaded = true;
        }
        return m_module;
    }

    void VulkanShader::ClearModule()
    {
        if (m_module != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(s_device, m_module, nullptr);
            isModuleLoaded = false;
        }
    }

    VkDevice vkclass::VulkanShaderPass::s_device = VK_NULL_HANDLE;

    VulkanShaderPass::VulkanShaderPass()
    {
        
    }

    VulkanShaderPass::~VulkanShaderPass()
    {
        for(auto& handle: m_descriptorLayoutHandles)
        {
            vkDestroyDescriptorSetLayout(s_device, handle.Layout, nullptr);
        }
        
        if(m_pipelineLayout != VK_NULL_HANDLE)
        {
            vkDestroyPipelineLayout(s_device, m_pipelineLayout, nullptr);
        }
    }

    void VulkanShaderPass::Prepare()
    {
        for(auto& shader : m_shaders)
        {
            switch (shader->GetStage())
            {
            case VX::ShaderStage::Vertex:   prepareVertexShader(shader);    break;
            case VX::ShaderStage::Fragment: prepareFragmentShader(shader);  break;
            default: VX_CORE_WARN("ShaderPass::Prepare: shader stage is not supported yet."); break;
            }
        }
        
        // prepare pipeline layouts from all shader modules
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        descriptorSetLayouts.reserve(m_descriptorLayoutHandles.size());

        for (const auto& handle : m_descriptorLayoutHandles) {
            descriptorSetLayouts.push_back(handle.Layout);
        }
        
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

//        std::vector<VkPushConstantRange> vkPushConstantRanges;
//        std::transform(m_pushConstants.begin(), m_pushConstants.end(), std::back_inserter(vkPushConstantRanges),
//            [](const std::pair<VkPushConstantRange, void*>& pair) {
//                return pair.first;
//            });
//        pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(m_pushConstants.size());
//        pipelineLayoutInfo.pPushConstantRanges = vkPushConstantRanges.data();
        
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        
        VK_CHECK_RESULT(vkCreatePipelineLayout(s_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));
    }

    std::vector<VX::Ref<VulkanShader>> VulkanShaderPass::GetVulkanShaders()
    {
        std::vector<VX::Ref<VulkanShader>> vulkanShaders;
        for(auto& shader : m_shaders)
        {
            vulkanShaders.push_back(std::static_pointer_cast<VulkanShader>(shader));
        }
        return vulkanShaders;
    }

    void VulkanShaderPass::prepareVertexShader(VX::Ref<VX::Shader> shader)
    {
        VX_CORE_INFO("ShaderPass: preparing vertex shader...");
        auto& inputLayout = shader->GetShaderLayout("Input");
        for (const auto& block : inputLayout)
        {
            if (block.Elements.size() != 1)
            {
                VX_CORE_WARN("VulkanShaderPass::prepareVertexShader: Vertex input data type is not supported yet...");
                // std::runtime_error("VulkanShaderPass: Vertex input data type is not supported...");
                return;
            }
            VkVertexInputAttributeDescription attribute{};
            attribute.binding = block.Binding;
            attribute.location = block.Location;
            attribute.offset = m_vertexStride;
            VX_CORE_TRACE("ShaderPass: binding: {0}, location: {1}, offset: {2}", block.Binding ,block.Location, attribute.offset);
            attribute.format = ShaderDataTypeToVulkanFormat(block.Elements[0].Type);

            m_attributeDescriptions.push_back(attribute);
            m_vertexStride += block.Stride;
        }

        // all per-vertex data is packed together in one array, so only one binding
        m_bindingDescription.binding = 0; // specify the index of binding in the array of bindings
        m_bindingDescription.stride = m_vertexStride; // number of bytes from one entry to the next
        m_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // per-vertex data, TODO: change this for particle system
    
        auto& uboLayout = shader->GetShaderLayout("UniformBuffers");
        for (const auto& uboBlock : uboLayout)
        {
            DescriptorLayoutHandle handle;
            handle.AddBinding(uboBlock.Binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
            handle.SetShaderStage(VulkanShaderStage(shader->GetStage()));
            handle.Build(s_device);
            m_descriptorLayoutHandles.push_back(handle);
        }
    }

    void VulkanShaderPass::prepareFragmentShader(VX::Ref<VX::Shader> shader)
    {
        auto& uboLayout = shader->GetShaderLayout("UniformBuffers");
        for (const auto& uboBlock : uboLayout)
        {
            DescriptorLayoutHandle handle;
            handle.AddBinding(uboBlock.Binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
            handle.SetShaderStage(VulkanShaderStage(shader->GetStage()));
            handle.Build(s_device);
            m_descriptorLayoutHandles.push_back(handle);
        }
    }

    void VulkanShaderPass::Init(VkDevice device)
    {
        s_device = device;
    }

    VkDevice vkclass::VulkanShaderEffect::s_device = VK_NULL_HANDLE;
//    VulkanRenderPassManager* VulkanShaderEffect::s_renderPassManager = nullptr;
    VulkanCommandManager* VulkanShaderEffect::s_commandBufferManager = nullptr;

    VulkanShaderEffect::VulkanShaderEffect(VX::Ref<VX::ShaderPass> shaderPass)
    {
//        m_shaderPass = shaderPass;
//        m_VulkanShaderPass = std::static_pointer_cast<VulkanShaderPass>(shaderPass);
    }

    VulkanShaderEffect::~VulkanShaderEffect()
    {
//        vkDestroyPipeline(s_device, m_pipeline, nullptr);
    }

    void VulkanShaderEffect::Build()
    {
//        m_builder.Clear();
//        for( auto& shader : m_VulkanShaderPass->GetVulkanShaders())
//        {
//            m_builder.AddShader(shader->GetModule(), VulkanShaderStageFlagBits(shader->GetStage()));
//        }
//        m_builder.SetVertexInput(m_VulkanShaderPass->GetVertexInputBinding(), m_VulkanShaderPass->GetVertexInputAttributes());
//        m_builder.SetCullMode(VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
//
//        switch (m_renderPassStage)
//        {
//        case VX::RenderPassStage::main:
//            m_pipeline = m_builder.BuildPipeline(m_VulkanShaderPass->GetPipelineLayout(), s_renderPassManager->GetRenderPass("main"));
//            break;
//        default:
//            break;
//        }
        
        // for( auto& shader : m_VulkanShaderPass->GetVulkanShaders())
        // {
        //     shader->ClearModule();
        // }
    }

    void VulkanShaderEffect::Bind()
    {
//        if (m_pipeline == VK_NULL_HANDLE)
//        {
//            VX_CORE_WARN("VulkanShaderEffect:: Binding null pipeline");
//        }
//        else
//        {
//            s_commandBufferManager->BindPipeline(m_pipeline);
//        }
    }

//    void VulkanShaderEffect::Init(VkDevice device, VulkanRenderPassManager* renderPassManager, VulkanCommandManager* commandBufferManager)
//    {
//        s_device = device;
//        s_renderPassManager = renderPassManager;
//        s_commandBufferManager = commandBufferManager;
//    }
}
