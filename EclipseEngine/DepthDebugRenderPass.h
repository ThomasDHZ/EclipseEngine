#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"

class DepthDebugRenderPass : public RenderPass
{
private:
	void BuildRenderPass();
	void BuildRenderPassPipelines();

	std::shared_ptr<GraphicsPipeline> DepthDebugPipeline;

public:
	DepthDebugRenderPass();
	~DepthDebugRenderPass();

	std::shared_ptr<RenderedColorTexture> DebugTexture;

	void StartUp(uint32_t textureSize);
	void RebuildSwapChain(uint32_t textureSize);
	void Draw();
	void Destroy();
};