#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GraphicsPipeline.h"
#include "MeshPicker3DPipeline.h"

class MeshPickerRenderPass3D : public RenderPass
{
private:
	MeshPicker3DPipeline MeshPickerPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	MeshPickerRenderPass3D();
	~MeshPickerRenderPass3D();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	void BuildRenderPass();
	VkCommandBuffer Draw();
	void Destroy();

	Pixel ReadPixel(glm::ivec2 PixelTexCoord);
};