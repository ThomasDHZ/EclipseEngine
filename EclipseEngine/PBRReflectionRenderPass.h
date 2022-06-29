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

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceMap, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterMap);

public:
	PBRReflectionRenderPass();
	~PBRReflectionRenderPass();

	std::shared_ptr<RenderedCubeMapTexture> ReflectionCubeMapTexture;
	std::shared_ptr<RenderedCubeMapTexture> BloomTexture;

	void BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceMap, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterMap, uint32_t cubeMapSize);
	VkCommandBuffer Draw();
	void Destroy();
};

