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

	std::shared_ptr<RenderedCubeMapDepthTexture> RenderPassDepthTexture;

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	DepthCubeMapRenderer();
	~DepthCubeMapRenderer();

	std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> DepthCubeMapTextureList;

	void BuildRenderPass(glm::vec2 TextureResolution);
	VkCommandBuffer Draw(std::vector<std::shared_ptr<PointLight>> PointLightList);
	void OneTimeDraw(glm::vec2 TextureResolution, std::vector<std::shared_ptr<PointLight>> PointLightList);
	void Destroy();
};