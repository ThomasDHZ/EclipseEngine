#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "SkyboxMesh.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"


class DepthRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<GraphicsPipeline> DepthPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	DepthRenderPass();
	~DepthRenderPass();

	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void BuildRenderPass();
	VkCommandBuffer Draw();
	void Destroy();
};