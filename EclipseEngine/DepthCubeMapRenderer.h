#pragma once
#include "RenderPass.h"
#include "DepthCubeMapPipeline.h"
#include "RenderedCubeMapDepthTexture.h"

class DepthCubeMapRenderer : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	DepthCubeMapPipeline depthCubeMapPipeline;
	JsonGraphicsPipeline depthCubeMapPipeline2;

	bool FirstDraw = true;

	void RenderPassDesc();
	void BuildRenderPassPipelines();
	void ClearTextureList();

public:
	DepthCubeMapRenderer();
	~DepthCubeMapRenderer();

	std::shared_ptr<RenderedCubeMapDepthTexture> RenderPassDepthTexture;
	std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> DepthCubeMapTextureList;

	void BuildRenderPass(glm::vec2 TextureResolution);
	void OneTimeDraw(glm::vec2 TextureResolution);

	VkCommandBuffer Draw();
	void Destroy();
};