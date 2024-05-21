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
        
     
        m_fontShader = VX::CreateRef<VulkanShader>("ImGuiFont", "Resources/VortexAPI/shaders/ui.vert.spv", "Resources/VortexAPI/shaders/ui.frag.spv");
        m_fontShader->SetVertexLayout(ImguiLayout);
        m_fontShader->SetTexture(m_fontTexture);
        m_fontShader->SetPushConstant(VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstBlock), &FontPushConstant);
        m_fontShader->Prepare();
        
        FontPushConstant.scale = {1.0f, 1.0f};
        FontPushConstant.translate = {1.0f, 1.0f};
        
        // updateBuffers();
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

        // updateBuffers();
        
         // VX::Renderer::Submit(m_fontShader ,m_vertexArray);
    }

    void VulkanUILayer::OnUpdateEnd()
    {
        ImGui::Render();
    }

    void VulkanUILayer::updateBuffers()
    {
        // Update buffers only if vertex or index count has been changed compared to current buffer size

        ImDrawData* imDrawData = ImGui::GetDrawData();

        VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
        VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

        if ((vertexBufferSize == 0) || (indexBufferSize == 0)) 
        {
            return;
        }

        if (m_vertexBuffer->GetBuffer() == VK_NULL_HANDLE || (vertexCount != imDrawData->TotalVtxCount))
        {
            m_vertexBuffer.reset();
            m_vertexBuffer = VX::CreateRef<VulkanVertexBuffer>();
            m_vertexBuffer->SetLayout(ImguiLayout);

            ImDrawVert* vtxDst = new ImDrawVert[imDrawData->TotalVtxCount];
            int offset = 0;
            for (int n = 0; n < imDrawData->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = imDrawData->CmdLists[n];
                memcpy(vtxDst + offset, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
                offset += cmd_list->VtxBuffer.Size;
            }
            m_vertexBuffer->SetData(vtxDst, imDrawData->TotalVtxCount);

            m_indexBuffer.reset();
            ImDrawVert* idxDst = new ImDrawVert[imDrawData->TotalIdxCount];
            int idxOffset = 0;
            for (int n = 0; n < imDrawData->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = imDrawData->CmdLists[n];
                memcpy(idxDst + idxOffset, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                idxOffset += cmd_list->IdxBuffer.Size;
            }

            /*m_indexBuffer = VX::CreateRef<VulkanIndexBuffer>(void* data, VkDeviceSize size, unsigned long count);

            m_vertexArray->AddVertexBuffer(m_vertexBuffer);
            m_vertexArray->SetIndexBuffer(m_indexBuffer);*/
        }
    }
}
