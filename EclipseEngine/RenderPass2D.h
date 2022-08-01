#pragma once
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LinePipeline.h"
#include "WireFramePipeline.h"
#include "Renderer2DPipeline.h"

class RenderPass2D : public RenderPass
{
private:
	Renderer2DPipeline renderer2DPipeline;
	LinePipeline drawLinePipeline;
	WireFramePipeline wireframePipeline;

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

