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
#include "CubeMapSamplerPipeline.h"
#include "BlinnPhongReflectionPipeline.h"

class BlinnPhongReflectionRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	BlinnPhongReflectionPipeline blinnphongPipeline;
	CubeMapSamplerPipeline skyboxPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<CubeMapTexture> cubemap, std::shared_ptr<RenderedDepthTexture> depthTexture);

public:
	BlinnPhongReflectionRenderPass();
	~BlinnPhongReflectionRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> ColorTexture;
	std::shared_ptr<RenderedCubeMapTexture> BloomTexture;

	void BuildRenderPass(std::shared_ptr<CubeMapTexture> cubemap, std::shared_ptr<RenderedDepthTexture> depthTexture,  uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};

