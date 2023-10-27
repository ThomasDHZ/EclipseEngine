#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapTexture.h"
#include "SceneManager.h"
#include "OutLinePipeline.h"
#include "PBRReflectionPipeline.h"
#include "PBRInstancedReflectionPipeline.h"
#include "PBRBakeReflectionPipeline.h"
#include "TextureBaker.h"
#include "JsonGraphicsPipeline.h"

struct PBRReflectionRendererInput
{
	uint32_t CubeMapSize = 128;
	glm::vec3 ReflectionPosition = glm::vec3(0.0f);
	std::shared_ptr<RenderedCubeMapTexture> CubeMapTexture = nullptr;
};

class PBRReflectionRenderPass : public RenderPass
{
private:
	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;
	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	VkVertexInputBindingDescription VertexInputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescription;

	JsonGraphicsPipeline pbrReflectionPipeline;
	JsonGraphicsPipeline spriteReflectionPipeline;
	//PBRInstancedReflectionPipeline pbrInstancedPipeline;
	JsonGraphicsPipeline skyboxPipeline;

	std::shared_ptr<RenderedCubeMapTexture> RenderedTexture;
	std::shared_ptr<RenderedCubeMapTexture> RenderedReflectionCubeMap;
	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;

	CubeMapSamplerBuffer ReflectionMapSampler;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);
	void ClearTextureList();
	void UpdateView(glm::vec3 reflectPoint);

public:
	PBRReflectionRenderPass();
	~PBRReflectionRenderPass();

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize);
	std::shared_ptr<RenderedCubeMapTexture> DrawSubmit(std::shared_ptr<RenderedCubeMapTexture> cubeMap, uint32_t cubeMapSize);
	std::shared_ptr<RenderedCubeMapTexture> PreRenderPass(PBRRenderPassTextureSubmitList& textures, uint32_t cubeMapSize, glm::vec3 reflectPoint);
	VkCommandBuffer Draw(glm::vec3 reflectPoint);
	void Destroy();

	std::shared_ptr<RenderedCubeMapTexture> GetRenderedReflectionCubeMap() { return RenderedReflectionCubeMap; };
};