#pragma once
#include "BaseRenderPass.h"
#include "Renderer2DPipeline.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"

class RenderPass2D : public BaseRenderPass
{
private:
	void CreateRenderPass();
	void CreateRendererFramebuffers();

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedColorTexture> renderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	std::shared_ptr<Renderer2DPipeline> renderer2DPipeline;

	void StartUp();
	void RebuildSwapChain();

	void Draw();
	void Destroy();
};

