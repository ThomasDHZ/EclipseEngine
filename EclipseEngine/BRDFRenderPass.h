#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "BRDFPipeline.h"

class BRDFRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	BRDFRenderPass();
	~BRDFRenderPass();

	BRDFPipeline brdfPipeline;

	void BuildRenderPass(uint32_t textureSize);
	void Draw();
	void Destroy();
};