#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "Skybox.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "RenderedCubeMapDepthTexture.h"

class DepthCubeMapRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<GraphicsPipeline> depthCubeMapPipeline;

	void BuildRenderPass();
	void BuildRenderPassPipelines();

public:
	DepthCubeMapRenderPass();
	~DepthCubeMapRenderPass();

	std::shared_ptr<RenderedCubeMapDepthTexture> depthCubeMapTexture;

	void StartUp(uint32_t cubeMapSize);
	void RebuildSwapChain(uint32_t cubeMapSize);

	void Draw();
	void Destroy();
};

