#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "Skybox.h"
#include "RenderedCubeMapTexture.h"


class PBRRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedColorTexture> BloomTexture;

	std::shared_ptr<GraphicsPipeline> skyboxPipeline;

	std::shared_ptr<Skybox> skybox;

	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> RenderedCubeMap);

public:
	PBRRenderPass();
	~PBRRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedColorTexture> RenderedBloomTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void StartUp(std::shared_ptr<RenderedCubeMapTexture> RenderedCubeMap);
	void RebuildSwapChain(std::shared_ptr<RenderedCubeMapTexture> RenderedCubeMap);

	void Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView);
	void Destroy();
};