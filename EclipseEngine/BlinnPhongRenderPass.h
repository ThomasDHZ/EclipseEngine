#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "GraphicsPipeline.h"

class BlinnPhongRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedColorTexture> BloomTexture;

	std::shared_ptr<GraphicsPipeline> blinnphongPipeline;
	std::shared_ptr<GraphicsPipeline> drawLinePipeline;
	std::shared_ptr<GraphicsPipeline> wireframePipeline;

	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedColorTexture> shadowMap);

public:
	BlinnPhongRenderPass();
	~BlinnPhongRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedColorTexture> RenderedBloomTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void StartUp(std::shared_ptr<RenderedColorTexture> shadowMap);
	void RebuildSwapChain(std::shared_ptr<RenderedColorTexture> shadowMap);

	void Draw(SceneProperties& sceneProperties);
	void Destroy();
};

