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

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	DepthCubeMapRenderer();
	~DepthCubeMapRenderer();

	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;

	void BuildRenderPass(glm::vec2 TextureResolution);
	VkCommandBuffer Draw(glm::vec3 CubeSamplerPos);
	void OneTimeDraw(glm::vec2 TextureResolution, glm::vec3 CubeSamplerPos);
	void Destroy();
};