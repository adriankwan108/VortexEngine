#include "VulkanUILayer.hpp"
#include "Application.hpp"

namespace vkclass
{
    VulkanUILayer::VulkanUILayer()
        : VX::UILayer()
    {
    }

    void VulkanUILayer::OnAttach()
    {
        VX_CORE_INFO("VulkanUILayer: OnAttach");
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        
        // configuration flags
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        // setup style
        ImGui::StyleColorsDark();
        
        // load fonts
        // io.FontGlobalScale
        
        // dimensions
        auto width = static_cast<float>(VX::Application::Get().GetWindow().GetWidth());
        auto height = static_cast<float>(VX::Application::Get().GetWindow().GetHeight());
        io.DisplaySize = ImVec2(width, height);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        
        // key mapping
        
        // link to our vulkan renderer
        
        // init resources
        unsigned char* fontData;
        int texWidth, texHeight;
        io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
        
        m_fontTexture = VX::CreateRef<VulkanTexture2D>();
        m_fontTexture->LoadFromData(fontData, texWidth, texHeight);
        
        VX::VertexShaderLayout imguiLayout = {
            {VX::ShaderDataType::Float2, "pos"},
            {VX::ShaderDataType::Float2, "uv"},
            {VX::ShaderDataType::U32, "col"}
        };
        m_fontShader = VX::CreateRef<VulkanShader>("ImGuiFont", "Resources/VortexAPI/shaders/ui.vert.spv", "Resources/VortexAPI/shaders/ui.frag.spv");
        m_fontShader->SetVertexLayout(imguiLayout);
        m_fontShader->SetTexture(m_fontTexture);
        m_fontShader->SetPushConstant(VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstBlock));
        m_fontShader->Prepare();
        
        FontPushConstant.scale = {1.0f, 1.0f};
        FontPushConstant.translate = {1.0f, 1.0f};
        
//        m_vertexBuffer = VX::CreateRef<VulkanVertexBuffer>();
//        m_indexBuffer = VX::CreateRef<VulkanIndexBuffer>();
//        m_vertexArray = VX::CreateRef<VulkanVertexArray>();
        
//        ImDrawData* imDrawData = ImGui::GetDrawData();
//        ImDrawVert* vtxDst;
//        ImDrawIdx* idxDst;
//        for (int n = 0; n < imDrawData->CmdListsCount; n++)
//        {
//            const ImDrawList* cmd_list = imDrawData->CmdLists[n];
//            memcpy(vtxDst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
//            memcpy(idxDst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
//            vtxDst += cmd_list->VtxBuffer.Size;
//            idxDst += cmd_list->IdxBuffer.Size;
//        }
//        m_vertexBuffer->SetLayout(imguiLayout);
//        m_vertexBuffer->SetData(vtxDst, imDrawData->CmdLists[0]->VtxBuffer.Size * sizeof(ImDrawVert));
//        m_indexBuffer->SetData(idxDst, imDrawData->CmdLists[0]->VtxBuffer.Size * sizeof(ImDrawVert), 0);
//
//        m_vertexArray->AddVertexBuffer(m_vertexBuffer);
//        m_vertexArray->SetIndexBuffer(m_indexBuffer);
//
//        updateBuffers();
    }

    void VulkanUILayer::OnDetach()
    {
        VX_CORE_INFO("VulkanUILayer: OnDetach");
        ImGui::DestroyContext();
    }

    void VulkanUILayer::OnEvent(VX::Event& event)
    {
        // input polling
        
        // resize event
    }
    

    void VulkanUILayer::OnUpdateStart()
    {
        ImGui::NewFrame();
        
        {
            ImGui::Begin("Testing");
            
            ImGui::End();
        }

        {
            ImGui::ShowDemoWindow();
        }
        
         VX::Renderer::Submit(m_fontShader ,m_vertexArray);
    }

    void VulkanUILayer::OnUpdateEnd()
    {
        ImGui::Render();
    }

    void VulkanUILayer::updateBuffers()
    {
        
    }
}
