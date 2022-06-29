#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"

class BRDFRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	BRDFRenderPass();
	~BRDFRenderPass();

	std::shared_ptr<GraphicsPipeline> brdfPipeline;

	void BuildRenderPass(uint32_t textureSize);
	void Draw();
	void Destroy();
};