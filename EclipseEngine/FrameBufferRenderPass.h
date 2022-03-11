#pragma once
#include "FrameBufferPipeline.h"
#include "RenderPass.h"

class FrameBufferRenderPass : public RenderPass
{
private:
	std::shared_ptr<FrameBufferPipeline> frameBufferPipeline;
	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass();
	void BuildRendererFramebuffers();
	void BuildRenderPassPipelines();

public:
	FrameBufferRenderPass();
	~FrameBufferRenderPass();

	void StartUp(std::shared_ptr<RenderedColorTexture> RenderedTexture);
	void RebuildSwapChain(std::shared_ptr<RenderedColorTexture> RenderedTexture);

	void Draw();
	void Destroy();
};