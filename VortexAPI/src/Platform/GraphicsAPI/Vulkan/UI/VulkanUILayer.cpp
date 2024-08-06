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
        
        VX_CORE_INFO("DisplaySize: {0}, {1}", io.DisplaySize.x, io.DisplaySize.y);
//        FontPushConstant.scale = {2.0f /io.DisplaySize.x, 2.0f /io.DisplaySize.y};
        FontPushConstant.scale = {2.0f / 1920, 2.0f /1080};
        FontPushConstant.translate = {-1.0f, -1.0f};
     
        /*m_fontShader = VX::CreateRef<VulkanShader>("ImGuiFont", "Resources/VortexAPI/shaders/ui.vert.spv", "Resources/VortexAPI/shaders/ui.frag.spv");
        m_fontShader->SetVertexLayout(ImguiLayout);
        m_fontShader->SetTexture(m_fontTexture);
        m_fontShader->SetPushConstant(VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstBlock), &FontPushConstant);
        m_fontShader->Prepare();*/
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
//
//        {
//            ImGui::Begin("Testing");
//            ImGui::Text("Camera");
//            ImGui::End();
//        }

        {
            ImGui::ShowDemoWindow();
        }
    }

    void VulkanUILayer::OnUpdateEnd()
    {
        ImGui::Render();
        
        updateBuffers();
       
        // VX::Renderer::Submit(m_fontShader ,m_vertexArray);
    }

    void VulkanUILayer::updateBuffers()
    {
        // Update buffers only if vertex or index count has been changed compared to current buffer size

        ImDrawData* imDrawData = ImGui::GetDrawData();

        VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
        VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

        bool updateVA = false;
        if ((vertexBufferSize == 0) || (indexBufferSize == 0)) 
        {
            return;
        }
        
        if(m_vertexBuffer == nullptr || (vertexCount != imDrawData->TotalVtxCount))
        {
            // recreate all buffers and array
            m_vertexBuffer.reset();
            
            ImDrawVert* vtxDst = new ImDrawVert[imDrawData->TotalVtxCount];
            int vtxOffset = 0;
            for (int n = 0; n < imDrawData->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = imDrawData->CmdLists[n];
                
                // vertex
                memcpy(vtxDst + vtxOffset, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
                vtxOffset += cmd_list->VtxBuffer.Size;
            }
            m_vertexBuffer = VX::VertexBuffer::Create(vtxDst, vtxOffset * sizeof(ImDrawVert), sizeof(ImDrawVert));
            //m_vertexBuffer->SetLayout(ImguiLayout);
            if(vtxDst != nullptr)
            {
                delete[] vtxDst;
                vtxDst = nullptr;
            }
            
            VX_CORE_TRACE("UI: vertex buffer created");
            vertexCount = imDrawData->TotalVtxCount;
            updateVA = true;
        }
        
        if(m_indexBuffer == nullptr || (indexCount != imDrawData->TotalIdxCount))
        {
            // recreate all buffers and array
            m_indexBuffer.reset();
            
            ImDrawVert* idxDst = new ImDrawVert[imDrawData->TotalIdxCount];
            int idxOffset = 0;
            for (int n = 0; n < imDrawData->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = imDrawData->CmdLists[n];
                
                // vertex
                memcpy(idxDst + idxOffset, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                idxOffset += cmd_list->IdxBuffer.Size;
            }
            m_indexBuffer = VX::IndexBuffer::Create(idxDst, idxOffset * sizeof(ImDrawIdx));

            // draw cmd info
            int32_t vertexOffset = 0;
            int32_t indexOffset = 0;
            for (int32_t i = 0; i < imDrawData->CmdListsCount; i++)
            {
                const ImDrawList* cmd_list = imDrawData->CmdLists[i];
                for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
                {
                    const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];
                    
                    auto x = std::max((int32_t)(pcmd->ClipRect.x), 0);
                    auto y = std::max((int32_t)(pcmd->ClipRect.y), 0);
                    auto width= (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
                    auto height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);
                    
                    m_indexBuffer->AddDrawCmd(pcmd->ElemCount, indexOffset, vertexOffset, x, y, width, height);
                    indexOffset += pcmd->ElemCount;
                }
                vertexOffset += cmd_list->VtxBuffer.Size;
            }
            
            if(idxDst != nullptr)
            {
                delete[] idxDst;
                idxDst = nullptr;
            }
            
            VX_CORE_TRACE("UI: index buffer created");
            indexCount = imDrawData->TotalIdxCount;
            updateVA = true;
        }

        if(updateVA == true)
        {
            // update vertex array
            m_vertexArray.reset();
            m_vertexArray = VX::VertexArray::Create();
            m_vertexArray->AddVertexBuffer(m_vertexBuffer);
            m_vertexArray->SetIndexBuffer(m_indexBuffer);
            VX_CORE_TRACE("UI: vertex array updated");
        }
    }
}
