#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "Skybox.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"


class DepthPassRendererPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<GraphicsPipeline> DepthPipeline;

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