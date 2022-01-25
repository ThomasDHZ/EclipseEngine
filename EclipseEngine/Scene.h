#pragma once
#include "Window.h"
#include "VulkanRenderer.h"
#include "InterfaceRenderPass.h"
#include "FrameBufferRenderPass.h"

class Scene
{
private:
	InterfaceRenderPass imGuiRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;
public:
	Scene();
	~Scene();

	void Update();
	void Draw();
};

