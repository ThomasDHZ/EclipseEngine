#pragma once
#include "Window.h"
#include "VulkanRenderer.h"
#include "InterfaceRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "UniformBuffer.h"

class Scene
{
private:
	struct SceneProperties
	{
		float Timer;
	};

	UniformBuffer<SceneProperties> SceneDataUniformBuffer;
	InterfaceRenderPass imGuiRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;

public:
	Scene();
	~Scene();

	void StartUp();
	void Update();
	void Draw();
};

