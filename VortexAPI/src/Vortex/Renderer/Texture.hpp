#pragma once

#include "VortexPCH.hpp"

namespace VX
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual const std::string& GetPath() = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(const std::string& path);
    };
}