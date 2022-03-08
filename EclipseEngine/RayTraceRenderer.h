#pragma once
#include "FrameBufferRenderPass.h"
#include "RayTraceRenderPass.h"

class RayTraceRenderer : public RenderPass
{
private:
public:
	RayTraceRenderer();
	~RayTraceRenderer();

	RayTraceRenderPass rayTraceRenderPass;
	FrameBufferRenderPass FrameBufferRenderer;

	void StartUp();
	void RebuildSwapChain();
	void GUIUpdate();
	void Draw(SceneProperties& sceneProperties, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};

