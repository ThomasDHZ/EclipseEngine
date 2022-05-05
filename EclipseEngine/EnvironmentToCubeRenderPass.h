#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "Skybox.h"
#include "RenderedCubeMapTexture.h"

class EnvironmentToCubeRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<GraphicsPipeline> EnvirnmentToCubePipeline;

	std::shared_ptr<Skybox> skybox;

	void BuildRenderPass();
	void CreateRendererFramebuffers();
	void BuildRenderPassPipelines();

public:
	EnvironmentToCubeRenderPass();
	~EnvironmentToCubeRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> RenderedCubeMap;

	void StartUp();
	void RebuildSwapChain();

	void Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView);
	void Destroy();
};