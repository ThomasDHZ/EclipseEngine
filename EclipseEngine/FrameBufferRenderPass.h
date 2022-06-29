#pragma once
#include "GraphicsPipeline.h"
#include "RenderPass.h"
#include "RenderedColorTexture.h"

class FrameBufferRenderPass : public RenderPass
{
private:
	std::shared_ptr<GraphicsPipeline> frameBufferPipeline;
	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void RenderPassDesc();
	void BuildRendererFramebuffers();
	void BuildRenderPassPipelines();

public:
	FrameBufferRenderPass();
	~FrameBufferRenderPass();

	void BuildRenderPass(std::shared_ptr<RenderedColorTexture> RenderedTexture);
	VkCommandBuffer Draw();
	void Destroy();
};