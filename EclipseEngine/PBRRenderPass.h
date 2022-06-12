#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "Skybox.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"


class PBRRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<RenderedCubeMapTexture> ReflectionIrradianceTexture;
	std::shared_ptr<RenderedCubeMapTexture> ReflectionPrefilterTexture;

	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedColorTexture> BloomTexture;

	std::shared_ptr<GraphicsPipeline> pbrPipeline;
	std::shared_ptr<GraphicsPipeline> skyboxPipeline;
	std::shared_ptr<GraphicsPipeline> drawLinePipeline;
	std::shared_ptr<GraphicsPipeline> wireframePipeline;

	void BuildRenderPass();
	void BuildRenderPassPipelines();

public:
	PBRRenderPass();
	~PBRRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedColorTexture> RenderedBloomTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void StartUp();
	void StartUp(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceTexture, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterTexture);
	void RebuildSwapChain();

	void Draw();
	void Destroy();
};