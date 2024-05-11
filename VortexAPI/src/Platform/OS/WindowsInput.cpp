#include "WindowsInput.hpp"

#include <GLFW/glfw3.h>
#include "Vortex/Application.hpp"

namespace VX
{
    WindowsInput::WindowsInput()
        :Input::Input()
    {

    }

    bool WindowsInput::IsKeyPressedFn(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedFn(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::GetMousePositionFn()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { static_cast<float>(xpos), static_cast<float>(ypos) };
    }

    float WindowsInput::GetMouseXFn()
    {
        auto [x, y] = GetMousePositionFn();
        return x;
    }

    float WindowsInput::GetMouseYFn()
    {
        auto [x, y] = GetMousePositionFn();
        return y;
    }
}
