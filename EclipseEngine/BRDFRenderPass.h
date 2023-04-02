#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "BRDFPipeline.h"
#include "JsonGraphicsPipeline.h"

class BRDFRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines();

	JsonGraphicsPipeline BRDFPipeline;

public:
	BRDFRenderPass();
	~BRDFRenderPass();

	void BuildRenderPass(uint32_t textureSize);
	void OneTimeDraw(uint32_t textureSize);
	VkCommandBuffer Draw();
	void Destroy();
};