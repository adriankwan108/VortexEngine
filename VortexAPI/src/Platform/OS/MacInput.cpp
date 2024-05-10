#include "MacInput.hpp"

#include <GLFW/glfw3.h>
#include "Vortex/Application.hpp"

namespace VX
{
    bool MacInput::IsKeyPressed(int keycode)
    {
        auto window = static_cast<GLFWwindow*>( Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || GLFW_REPEAT;
    }
}
