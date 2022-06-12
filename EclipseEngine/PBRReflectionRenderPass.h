#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "Skybox.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"

class PBRReflectionRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	std::shared_ptr<GraphicsPipeline> pbrPipeline;
	std::shared_ptr<GraphicsPipeline> skyboxPipeline;
	std::shared_ptr<GraphicsPipeline> drawLinePipeline;
	std::shared_ptr<GraphicsPipeline> wireframePipeline;

	void BuildRenderPass();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceMap, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterMap);

public:
	PBRReflectionRenderPass();
	~PBRReflectionRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> ReflectionCubeMapTexture;
	std::shared_ptr<RenderedCubeMapTexture> BloomTexture;

	void StartUp(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceMap, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterMap);
	void RebuildSwapChain(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceMap, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterMap);

	void Draw();
	void Destroy();
};

