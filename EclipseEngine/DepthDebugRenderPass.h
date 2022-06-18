#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"

class DepthDebugRenderPass : public RenderPass
{
private:
	void BuildRenderPass();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedDepthTexture> depthTexture);

	std::shared_ptr<GraphicsPipeline> DepthDebugPipeline;

public:
	DepthDebugRenderPass();
	~DepthDebugRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void StartUp(std::shared_ptr<RenderedDepthTexture> depthTexture);
	void RebuildSwapChain(std::shared_ptr<RenderedDepthTexture> depthTexture);
	void Draw();
	void Destroy();
};