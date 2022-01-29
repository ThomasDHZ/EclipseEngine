#pragma once
#include "BaseRenderPass.h"
#include "Renderer2DPipeline.h"
#include "RenderedTexture.h"

class RenderPass2D : public BaseRenderPass
{
private:
	void CreateRenderPass();
	void CreateRendererFramebuffers();

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedTexture> renderedTexture;
	std::shared_ptr<RenderedTexture> depthTexture;

	std::shared_ptr<Renderer2DPipeline> renderer2DPipeline;

	void StartUp();
	void RebuildSwapChain();

	void Draw();
	void Destroy();
};

