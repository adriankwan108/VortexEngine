#pragma once
#include "VortexPCH.hpp"
#include "Vortex/UI/UILayer.hpp"
#include <imgui.h>

namespace vkclass
{
    class VulkanUILayer : public VX::UILayer
    {
    public:
        VulkanUILayer();
        ~VulkanUILayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(VX::Timestep ts) override {};
        virtual void OnEvent(VX::Event& event) override;

        virtual void OnUpdateStart() override;
        virtual void OnUpdateEnd() override;

    };
}
