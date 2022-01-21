#include <iostream>
#include "VulkanRenderer.h"

int main()
{
    Window window = Window();
    window.CreateWindow(1920, 1080, "Eclipse Engine");

    VulkanRenderer renderer = VulkanRenderer(window);
    while (!glfwWindowShouldClose(window.GetWindowPtr()))
    {
        glfwPollEvents();
        GameController::IsButtonPressed(GLFW_GAMEPAD_BUTTON_CROSS);
        GameController::IsJoyStickMoved(GLFW_GAMEPAD_BUTTON_CROSS);
        GameController::IsJoyStickMoved(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER);
    }
}

