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
	void ClearTextureList();

public:
	DepthCubeMapRenderer();
	~DepthCubeMapRenderer();

	std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> DepthCubeMapTextureList;

	void BuildRenderPass(std::vector<std::shared_ptr<PointLight>> PointLightList, glm::vec2 TextureResolution);
	VkCommandBuffer Draw(std::vector<std::shared_ptr<PointLight>> PointLightList);
	void OneTimeDraw(std::vector<std::shared_ptr<PointLight>> PointLightList, glm::vec2 TextureResolution);
	void Destroy();
};