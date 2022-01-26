#pragma once
#include "Window.h"
#include "VulkanRenderer.h"
#include "InterfaceRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "UniformBuffer.h"
#include "Texture.h"

class Scene
{
private:
	struct SceneProperties
	{
		float Timer;
	};

	Texture texture;
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

