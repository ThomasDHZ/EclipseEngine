#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"

class BRDFRenderPass : public RenderPass
{
private:
	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines();

public:
	BRDFRenderPass();
	~BRDFRenderPass();

	std::shared_ptr<GraphicsPipeline> brdfPipeline;

	void StartUp(uint32_t textureSize);
	void RebuildSwapChain(uint32_t textureSize);
	void Draw();
	void Destroy();
};