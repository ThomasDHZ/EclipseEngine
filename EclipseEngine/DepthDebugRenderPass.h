#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "DepthDebugPipeline.h"

class DepthDebugRenderPass : public RenderPass
{
private:
	void BuildRenderPass();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedDepthTexture> depthTexture);

	DepthDebugPipeline depthDebugPipeline;

public:
	DepthDebugRenderPass();
	~DepthDebugRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(std::shared_ptr<RenderedDepthTexture> depthTexture);
	VkCommandBuffer Draw();
	void Destroy();
};