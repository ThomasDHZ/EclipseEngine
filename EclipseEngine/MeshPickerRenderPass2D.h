#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GraphicsPipeline.h"
#include "MeshPicker2DPipeline.h"

class MeshPickerRenderPass2D : public RenderPass
{
private:
	MeshPicker2DPipeline MeshPickerPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	MeshPickerRenderPass2D();
	~MeshPickerRenderPass2D();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	void BuildRenderPass();
	VkCommandBuffer Draw();
	void Destroy();

	Pixel ReadPixel(glm::ivec2 PixelTexCoord);
};