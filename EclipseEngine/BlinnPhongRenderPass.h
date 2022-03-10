#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GameObjectManager.h"
#include "BlinnPhongPipeline.h"
#include "DrawLinePipeline.h"
#include "WireframePipeline.h"

class BlinnPhongRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedColorTexture> BloomTexture;

	std::shared_ptr<BlinnPhongPipeline> blinnphongPipeline;
	std::shared_ptr<DrawLinePipeline> drawLinePipeline;
	std::shared_ptr<WireframePipeline> wireframePipeline;

	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines();

public:
	BlinnPhongRenderPass();
	~BlinnPhongRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedColorTexture> RenderedBloomTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void StartUp();
	void RebuildSwapChain();

	void Draw(SceneProperties sceneProperties);
	void Destroy();
};

