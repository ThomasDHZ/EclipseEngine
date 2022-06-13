#pragma once
#include "RenderPass.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class DepthPassRendererPass : RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<GraphicsPipeline> depthPipeline;

	void BuildRenderPass();
	void BuildRenderPassPipelines();

public:
	DepthPassRendererPass();
	~DepthPassRendererPass();

	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void StartUp();
	void RebuildSwapChain();

	void Draw();
	void Destroy();
};

