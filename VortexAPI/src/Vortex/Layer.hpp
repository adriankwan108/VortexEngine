#pragma once

#include "Events/Event.hpp"
#include "Core/Timestep.hpp"

namespace VX
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }

    private:
        std::string m_DebugName;
    };
}
