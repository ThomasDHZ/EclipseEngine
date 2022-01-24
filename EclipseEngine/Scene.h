#pragma once
#include "Window.h"
#include "VulkanRenderer.h"
#include "InterfaceRenderPass.h"
class Scene
{
private:
	InterfaceRenderPass imGuiRenderPass;
	std::vector<VkCommandBuffer> CommandBufferSubmitList;
public:
	Scene();
	~Scene();

	void Update();
};

