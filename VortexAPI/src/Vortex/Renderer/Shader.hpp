#pragma once

#include "VortexPCH.hpp"
#include "ShaderLayout.hpp"
#include "Texture.hpp"
#include <map>

#include <spirv_cross/spirv_glsl.hpp>

namespace VX
{
    // api agnostic
    //class Shader
    //{
    //public:
    //    virtual ~Shader() = default;

    //    virtual void Bind() = 0;
    //    virtual void UnBind() const = 0;

    //    virtual void SetVertexLayout(VX::VertexShaderLayout layout) = 0;
    //    
    //    // grouped by update frequency
    //    virtual void SetGlobalLayout    (int binding, VX::UniformShaderLayout layout) = 0; //   set 0: global (per frame)
    //    virtual void SetPassLayout      (int binding, VX::UniformShaderLayout layout) = 0; //   set 1: per pass (shadows)
    //    virtual void SetTexture         (Ref<Texture2D> texture)                      = 0; //   set 2: per material
    //    virtual void SetObjectLayout    (int binding, VX::UniformShaderLayout layout) = 0; //   set 3: per object (transform)

    //    // testing
    //    // set 0 global layout
    //    // set 1 view resource layout
    //    // set 2 material layout // i.e. textures, material parameters
    //    // set 3 object transforms layout
    //    // set pushconstant // shader control values

    //    
    //    virtual void Prepare() = 0;
    //    
    //    virtual const std::string& GetName() const = 0;
    //    
    //    static Ref<Shader> Create(const std::string& name, const std::string& vertexFilePath, const std::string& fragFilePath);
    //};

    enum class ShaderStage
    {
        None = 0,
        Vertex,
        Fragment,
        Tessellation,
        Geometry,
        Compute
    };

    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool, U32
    };

    static uint32_t ShaderDataTypeSize(VX::ShaderDataType type)
    {
        switch (type)
        {
        case VX::ShaderDataType::Float:    return 4;
        case VX::ShaderDataType::Float2:   return 4 * 2;
        case VX::ShaderDataType::Float3:   return 4 * 3;
        case VX::ShaderDataType::Float4:   return 4 * 4;
        case VX::ShaderDataType::Mat3:     return 4 * 3 * 3;
        case VX::ShaderDataType::Mat4:     return 4 * 4 * 4;
        case VX::ShaderDataType::Int:      return 4;
        case VX::ShaderDataType::Int2:     return 4 * 2;
        case VX::ShaderDataType::Int3:     return 4 * 3;
        case VX::ShaderDataType::Int4:     return 4 * 4;
        case VX::ShaderDataType::Bool:     return 1;
        case VX::ShaderDataType::U32:      return 4;
        default: VX_CORE_ASSERT(false, "Layout: Unknown Shader data type.");
        }
    }

    static VX::ShaderDataType SpirTypeToShaderDataType(spirv_cross::SPIRType type)
    {
        auto& columnSize = type.columns;
        auto& vecSize = type.vecsize;
        
        if(columnSize == 1)
        {
            // vector
            if (type.basetype == spirv_cross::SPIRType::Float)
            {
                VX_CORE_TRACE("GetShaderDataType: Float{0}",vecSize);
                switch (vecSize)
                {
                    case 1: return VX::ShaderDataType::Float;
                    case 2: return VX::ShaderDataType::Float2;
                    case 3: return VX::ShaderDataType::Float3;
                    case 4: return VX::ShaderDataType::Float4;
                    default: return VX::ShaderDataType::None;
                }
            }else if(type.basetype == spirv_cross::SPIRType::Int)
            {
                VX_CORE_TRACE("GetShaderDataType: Int{0}",vecSize);
                switch (vecSize)
                {
                    case 1: return VX::ShaderDataType::Int;
                    case 2: return VX::ShaderDataType::Int2;
                    case 3: return VX::ShaderDataType::Int3;
                    case 4: return VX::ShaderDataType::Int4;
                    default: return VX::ShaderDataType::None;
                }
            }
        }else if(columnSize > 1)
        {
            // matrix
            VX_CORE_TRACE("GetShaderDataType: Mat{0}",vecSize);
            switch (vecSize)
            {
                case 3: return VX::ShaderDataType::Mat3;
                case 4: return VX::ShaderDataType::Mat4;
                default: return VX::ShaderDataType::None;
            }
        }else if(columnSize == 0)
        {
            // basic type
        }
        
        VX_CORE_TRACE("GetShaderDataType: None");
        return VX::ShaderDataType::None;
    }

    struct ShaderElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;

        ShaderElement() = default;
        ShaderElement(ShaderDataType type, const std::string& name)
            : Type(type), Name(name), Size(ShaderDataTypeSize(type))
        {
        }
    };

    struct ShaderBlock
    {
        int Location;
        int Set;
        int Binding;
        std::vector<ShaderElement> Elements;
        
        ShaderBlock() = default;
        ShaderBlock(int location, int set, int binding, const std::vector<ShaderElement>& elements)
            : Location(location), Set(set), Binding(binding), Elements(elements)
        {
        }
    };

    /*
    * An api-agnostic shader, loading targeted shader path, and reflect elements (paramaters inside shader)
    * In Vulkan, this also manage shader module, descriptorSetLayout
    */
    class Shader
    {
    public:
        static Ref<Shader> Create(const std::string& name, const std::string& filePath, ShaderStage stage = ShaderStage::None);

    protected:
        std::string m_name;
        std::string m_filePath;
        ShaderStage m_stage;
        
        std::map<std::string, std::vector<ShaderBlock>> m_layoutGroups;
    };

    class ShaderPass
    {
    public:
        static Ref<ShaderPass> Create();
        
        void AddShader(VX::Ref<Shader> shader);
        virtual void Prepare() = 0;
        // pipeline settings
    protected:
        std::vector<VX::Ref<Shader>> m_shaders;
        // vk pipeline
    };

    class ShaderEffect
    {
    public:


    private:
        std::vector<ShaderPass> m_shaderPasses;
    };
}
