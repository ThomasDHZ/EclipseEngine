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
	std::vector<VkCommandBuffer> CommandBufferSubmitList;
public:
	Scene();
	~Scene();

	void Update();
};

