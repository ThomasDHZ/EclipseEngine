#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "GraphicsPipeline.h"

class GBufferRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<GraphicsPipeline> hybridPipeline;

	void BuildRenderPass();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedColorTexture> shadowMap);

public:
	GBufferRenderPass();
	~GBufferRenderPass();

	std::shared_ptr<RenderedColorTexture> PositionTexture;
	std::shared_ptr<RenderedColorTexture> TangentTexture;
	std::shared_ptr<RenderedColorTexture> BiTangentTexture;
	std::shared_ptr<RenderedColorTexture> TBNormalTexture;
	std::shared_ptr<RenderedColorTexture> NormalTexture;
	std::shared_ptr<RenderedColorTexture> AlbedoTexture;
	std::shared_ptr<RenderedColorTexture> SpecularTexture;
	std::shared_ptr<RenderedColorTexture> BloomTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void StartUp(std::shared_ptr<RenderedColorTexture> shadowMap);
	void RebuildSwapChain(std::shared_ptr<RenderedColorTexture> shadowMap);

	VkCommandBuffer Draw();
	void Destroy();
};

