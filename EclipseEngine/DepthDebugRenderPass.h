#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"

class DepthDebugRenderPass : public RenderPass
{
private:
	void BuildRenderPass();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedDepthTexture> depthTexture);

	JsonGraphicsPipeline DepthDebugPipeline;

public:
	DepthDebugRenderPass();
	~DepthDebugRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(std::shared_ptr<RenderedDepthTexture> depthTexture);
	VkCommandBuffer Draw();
	void Destroy();
};