#pragma once
#include "Vortex/Layer.hpp"

namespace VX
{
    class UILayer : public Layer
    {
    public:
        UILayer();
        ~UILayer() = default;

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnUpdate(Timestep ts) override {};
        virtual void OnEvent(Event& event) = 0;

        virtual void OnUpdateStart() = 0;
        virtual void OnUpdateEnd() = 0;

        static UILayer* Create();
    };
}