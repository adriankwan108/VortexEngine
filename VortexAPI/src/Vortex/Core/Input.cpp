#include "Input.hpp"

namespace VX
{
    Input* Input::s_instance = nullptr;

    void Input::Set(Input* instance)
    {
        s_instance = instance;
    }
}
