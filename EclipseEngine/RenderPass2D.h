#pragma once
#include "RenderPass.h"
#include "Renderer2DPipeline.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GameObjectManager.h"
#include "DrawLinePipeline.h"
#include "WireframePipeline.h"

class RenderPass2D : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;

	std::shared_ptr<Renderer2DPipeline> renderer2DPipeline;
	std::shared_ptr<DrawLinePipeline> drawLinePipeline;
	std::shared_ptr<WireframePipeline> wireframePipeline;

	void CreateRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines();

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedColorTexture> renderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	void StartUp();
	void RebuildSwapChain();

	void Draw(SceneProperties sceneProperties);
	void Destroy();
};

