#pragma once
#include "FrameBufferPipeline.h"
#include "RenderPass.h"

class FrameBufferRenderPass : public RenderPass
{
private:
	std::shared_ptr<FrameBufferPipeline> frameBufferPipeline;

	void CreateRenderPass();
	void CreateRendererFramebuffers();

public:
	FrameBufferRenderPass();
	~FrameBufferRenderPass();

	void StartUp(std::shared_ptr<RenderedColorTexture> RenderedTexture);
	void RebuildSwapChain(std::shared_ptr<RenderedColorTexture> RenderedTexture);
	void Draw();
	void Destroy();
};