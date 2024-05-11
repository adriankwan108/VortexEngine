#include "MacInput.hpp"

#include <GLFW/glfw3.h>
#include "Vortex/Application.hpp"

namespace VX
{
    MacInput::MacInput()
        :Input::Input()
    {

    }

    bool MacInput::IsKeyPressedFn(int keycode)
    {
        auto window = static_cast<GLFWwindow*>( Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool MacInput::IsMouseButtonPressedFn(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> MacInput::GetMousePositionFn()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { static_cast<float>(xpos), static_cast<float>(ypos) };
    }

    float MacInput::GetMouseXFn()
    {
        auto [x, y] = GetMousePositionFn();
        return x;
    }

    float MacInput::GetMouseYFn()
    {
        auto [x, y] = GetMousePositionFn();
        return y;
    }
}
