#pragma once
#include "BaseRenderPass.h"
#include "Renderer2DPipeline.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GameObjectManager.h"
#include "DrawLinePipeline.h"
#include "WireframePipeline.h"

class RenderPass2D : public BaseRenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;

	void CreateRenderPass();
	void CreateRendererFramebuffers();

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedColorTexture> renderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	std::shared_ptr<Renderer2DPipeline> renderer2DPipeline;
	std::shared_ptr<DrawLinePipeline> drawLinePipeline;
	std::shared_ptr<WireframePipeline> wireframePipeline;

	void StartUp();
	void RebuildSwapChain();

	void Draw(SceneProperties sceneProperties);
	void Destroy();
};

