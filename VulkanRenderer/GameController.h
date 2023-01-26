#pragma once
#include <memory>
#include <GLFW\glfw3.h>
#include <iostream>

class GameController
{
private:
	static std::unique_ptr<GameController> Controller;
    static GLFWgamepadstate ControllerState;
    static bool ButtonPressed[16];
    static bool AxisMoved[16];

public:

	static void SetUpGamePad()
	{
        if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
		{
			const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);
		}

		int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	}

	static float IsJoyStickMoved(int axis) 
	{ 
        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &ControllerState))
        {
            if (axis == GLFW_GAMEPAD_AXIS_LEFT_TRIGGER ||
                axis == GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER)
            {
                if (ControllerState.axes[axis] != -1)
                {
                    return 1.0f;
                }
                else
                {
                    return 0.0f;
                }
            }
            else
            {
                if (ControllerState.axes[axis] >= 0.1 ||
                    ControllerState.axes[axis] <= -0.1)
                {
                    return ControllerState.axes[axis];
                }
                else
                {
                    return 0.0f;
                }
            }
        }
	}

	static bool IsButtonPressed(int Button) 
	{
        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &ControllerState))
        {
            return ControllerState.buttons[Button];
        }
	}
};