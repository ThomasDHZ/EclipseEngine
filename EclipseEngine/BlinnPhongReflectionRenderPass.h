#pragma once
#include "RenderPass.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "GraphicsPipeline.h"
#include "SkyboxMesh.h"
#include "Renderer3DPipeline.h"
#include "SkyboxPipeline.h"
#include "LinePipeline.h"
#include "WireFramePipeline.h"
#include "BillboardPipeline.h"

class BlinnPhongReflectionRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedColorTexture> BloomTexture;

	Renderer3DPipeline blinnphongPipeline;
	BillboardPipeline billBoardPipeline;
	SkyboxPipeline skyboxPipeline;
	LinePipeline linePipeline;
	WireFramePipeline wireframePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<CubeMapTexture> cubemap, std::shared_ptr<RenderedDepthTexture> depthTexture);

public:
	BlinnPhongReflectionRenderPass();
	~BlinnPhongReflectionRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedColorTexture> RenderedBloomTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void BuildRenderPass(std::shared_ptr<CubeMapTexture> cubemap, std::shared_ptr<RenderedDepthTexture> depthTexture);
	VkCommandBuffer Draw();
	void Destroy();
};

