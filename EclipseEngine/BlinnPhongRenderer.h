#pragma once
#include "BlinnPhongRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "InterfaceRenderPass.h"
class BlinnPhongRenderer
{
private:
	BlinnPhongRenderPass blinnPhongRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;
public:
	BlinnPhongRenderer();
	~BlinnPhongRenderer();

	void StartUp();
	void Update();
	void RebuildRenderers();
	void Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};

