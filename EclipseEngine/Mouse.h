#ifndef Mouse_H
#define Mouse_H
#pragma once
#include <GLFW\glfw3.h>
#include <memory>

class Mouse
{
private:
	static const uint32_t MOUSEBUTTONS = 3;

	static std::unique_ptr<Mouse> mouse;
	static double ScrollX;
	static double ScrollY;
	static double XPos;
	static double YPos;
	static double LastX;
	static double LastY;

	static bool MouseButtonPressed[3];
	static bool IsDragging;

public:
	static void MousePosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		mouse->LastX = mouse->XPos;
		mouse->LastY = mouse->YPos;
		mouse->XPos = xpos;
		mouse->YPos = ypos;
		mouse->IsDragging = mouse->MouseButtonPressed[0] || mouse->MouseButtonPressed[1] || mouse->MouseButtonPressed[2];
	}

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			if (button < MOUSEBUTTONS)
			{
				mouse->MouseButtonPressed[button] = true;
			}
		}
		else if (action == GLFW_RELEASE)
		{
			if (button < MOUSEBUTTONS)
			{
				mouse->MouseButtonPressed[button] = false;
				mouse->IsDragging = false;
			}
		}
	}

	static void MouseScrollCallback(GLFWwindow* window, double Xoffset, double Yoffset)
	{
		mouse->ScrollX = Xoffset;
		mouse->ScrollY = Yoffset;
	}

	static glm::vec2 GetMouseCoords()
	{
		return glm::vec2(XPos, YPos);
	}

	static glm::vec2 GetLastMouseCoords()
	{
		return glm::vec2(LastX, LastY);
	}

	static glm::vec2 GetMouseScrollBar()
	{
		return glm::vec2(ScrollX, ScrollY);
	}

	static void SetLastMouseCoords(glm::vec2 newLastCoords)
	{
		LastX = newLastCoords.x;
		LastY = newLastCoords.y;
	}


	static bool GetMouseButtonPressed(int button)
	{
		return MouseButtonPressed[button];
	}
};


#endif