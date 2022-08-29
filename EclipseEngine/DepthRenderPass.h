#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "SkyboxMesh.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "DepthPipeline.h"


class DepthRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<RenderedDepthTexture> RenderPassDepthTexture;
	DepthPipeline depthPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	DepthRenderPass();
	~DepthRenderPass();

	std::vector<std::shared_ptr<RenderedDepthTexture>> DepthTextureList;

	void BuildRenderPass(glm::vec2 TextureResolution);
	VkCommandBuffer Draw();
	void OneTimeDraw(glm::vec2 TextureResolution);
	void Destroy();
};