#pragma once
#include "RenderPass2D.h"
#include "FrameBufferRenderPass.h"
#include "InterfaceRenderPass.h"
class Renderer2D
{
private:
	RenderPass2D renderPass2D;
	FrameBufferRenderPass frameBufferRenderPass;
public:
	Renderer2D();
	~Renderer2D();

	void StartUp();
	void Update();
	void RebuildRenderers();
	void Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};

