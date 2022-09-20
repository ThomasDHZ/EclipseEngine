#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "SkyboxMesh.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "PBRPipeline.h"
#include "SkyboxPipeline.h"
#include "LinePipeline.h"
#include "WireFramePipeline.h"
#include "OutLinePipeline.h"
#include "PBRInstancePipeline.h"


class PBRRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<RenderedColorTexture> ColorTexture;

	PBRPipeline pbrPipeline;
	PBRInstancePipeline pbrInstancePipeline;
	SkyboxPipeline skyboxPipeline;
	LinePipeline linePipeline;
	OutLinePipeline outLinePipeline;
	WireFramePipeline wireframePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	PBRRenderPass();
	~PBRRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw();
	void Destroy();
};