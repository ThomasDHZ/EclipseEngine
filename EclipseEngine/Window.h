#ifndef Window_H
#define Window_H
#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include "Keyboard.h"
#include "Mouse.h"
#include "GameController.h"

class Window
{
private:

	GLFWwindow* GLFWindow;
	uint32_t Width;
	uint32_t Height;
	bool FramebufferResized;

public:

	void CreateWindow(uint32_t width, uint32_t height, const char* WindowName)
	{
		FramebufferResized = false;
		Width = width;
		Height = height;

		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		GLFWindow = glfwCreateWindow(width, height, WindowName, nullptr, nullptr);
		glfwSetWindowUserPointer(GLFWindow, this);
		glfwSetFramebufferSizeCallback(GLFWindow, frameBufferResizeCallBack);
		glfwSetCursorPosCallback(GLFWindow, Mouse::MousePosCallback);
		glfwSetMouseButtonCallback(GLFWindow, Mouse::MouseButtonCallback);
		glfwSetScrollCallback(GLFWindow, Mouse::MouseScrollCallback);
		glfwSetKeyCallback(GLFWindow, Keyboard::KeyCallBack);
		GameController::SetUpGamePad();
	}

	static void frameBufferResizeCallBack(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		app->FramebufferResized = true;
	}

	void Update()
	{

	}

	void Destroy()
	{
		glfwDestroyWindow(GLFWindow);
		glfwTerminate();
	}
	
	 GLFWwindow* GetWindowPtr() { return GLFWindow; }
	 unsigned int GetWindowWidth() { return Width; }
	 unsigned int GetWindowHeight() { return Height; }
	 glm::ivec2 GetWindowResolution() { return glm::ivec2(Width, Height); }
	 bool GetFrameBufferResizedFlag() { return FramebufferResized; }
};
#endif