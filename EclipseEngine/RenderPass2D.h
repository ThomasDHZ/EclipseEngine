#pragma once
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LinePipeline.h"

class RenderPass2D : public RenderPass
{
private:
	std::shared_ptr<GraphicsPipeline> renderer2DPipeline;
	std::shared_ptr<GraphicsPipeline> drawLinePipeline;
	std::shared_ptr<GraphicsPipeline> wireframePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedColorTexture> renderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	void BuildRenderPass();
	VkCommandBuffer Draw();
	void Destroy();
};

